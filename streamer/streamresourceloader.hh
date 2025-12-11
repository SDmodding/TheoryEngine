#pragma once

typedef void(__fastcall* FNFileLoadCallback)(const char*, bool, void*);

namespace UFG
{
	typedef bool(__fastcall* FNChunkFilter)(qChunk*);

	class StreamResourceLoader
	{
	public:
		struct LoadedFile : qNodeRB<LoadedFile>
		{
			enum STATE
			{
				STATE_LOADING,
				STATE_LOADED,
				STATE_UNKNOWN,
				STATE_ERROR
			};

			STATE meState = STATE_UNKNOWN;
			FNFileLoadCallback mfnCallback = 0;
			void* mCallbackParam = 0;
			qString mFilename;
			void* mData = 0;
			u32 mDataSize = 0;
			u32 mAllocFlags = 0;
			qMemoryPool* mpPool = 0;
			FNChunkFilter mfnChunkFilter = 0;

			LoadedFile(const char* filename) : qNodeRB(qStringHash32(filename)), mFilename(filename) {}
		};

		THEORY_SINLINE qTreeRB<LoadedFile> smLoadedFiles;

		static u32 Load(void* buffer, u32 num_bytes, const char* debug_identifier = 0, StreamerMetrics::DATA_TYPE data_type = StreamerMetrics::DATA_UNKNOWN, FNChunkFilter fnFilter = 0);

		static bool LoadResourceFile(const char* filename, qMemoryPool* pool = 0, u32 alloc_flags = 0, FNChunkFilter filter = 0, FNFileLoadCallback callback = 0, void* callbackParam = 0);
		
		static bool UnloadResourceFile(const char* filename);

		static int LoadInternal(LoadedFile* loaded_file);

		static void UnloadInternal(LoadedFile* loaded_file);
	};

#ifdef THEORY_IMPL

	u32 StreamResourceLoader::Load(void* buffer, u32 num_bytes, const char* debug_identifier, StreamerMetrics::DATA_TYPE data_type, FNChunkFilter fnFilter)
	{
		// TODO: Check for data_type and do some handling?

		auto resourceWarehouse = qResourceWarehouse::Instance();

		void* buffer_end = reinterpret_cast<void*>(reinterpret_cast<uptr>(buffer) + num_bytes);

		u32 offset = 0;
		while (num_bytes > offset)
		{
			auto chunk = reinterpret_cast<qChunk*>(reinterpret_cast<uptr>(buffer) + offset);
			if (chunk->mUID && (!fnFilter || fnFilter(chunk)))
			{
				auto resource = chunk->GetData();

				// Additional sanity check since "GetData()" can be out of bounds...
				if (buffer_end > resource)
				{
					if (auto inventory = resourceWarehouse->GetInventory(resource->mTypeUID)) {
						inventory->Add(resource);
					}
				}
			}

			offset += qAlignUp<u32>(chunk->mChunkSize, 4) + sizeof(qChunk);
		}

		return offset;
	}

	bool StreamResourceLoader::LoadResourceFile(const char* filename, qMemoryPool* pool, u32 alloc_flags, FNChunkFilter filter, FNFileLoadCallback callback, void* callbackParam)
	{
		auto loaded_file = new ("StreamResourceLoader::LoadedFile") LoadedFile(filename);
		if (!loaded_file) {
			return false;
		}

		loaded_file->mpPool = pool;
		loaded_file->mAllocFlags = alloc_flags;
		loaded_file->mfnChunkFilter = filter;
		loaded_file->mfnCallback = callback;
		loaded_file->mCallbackParam = callbackParam;

		if (callback)
		{
			// TODO: Implement this...
		}
		else if (LoadInternal(loaded_file))
		{
			smLoadedFiles.Add(loaded_file);
			return true;
		}

		qDelete(loaded_file);
		return false;
	}

	bool StreamResourceLoader::UnloadResourceFile(const char* filename)
	{
		for (auto loaded_file : smLoadedFiles)
		{
			if (qStringCompareInsensitive(loaded_file->mFilename, filename)) {
				continue;
			}

			UnloadInternal(loaded_file);
			smLoadedFiles.Delete(loaded_file);
			return true;
		}

		return false;
	}

	void StreamResourceLoader::UnloadInternal(LoadedFile* loaded_file)
	{
		auto data = loaded_file->mData;
		if (!data) {
			return;
		}

		auto resourceWarehouse = qResourceWarehouse::Instance();
		const u32 num_bytes = loaded_file->mDataSize;

		u32 offset = 0;
		while (num_bytes > offset)
		{
			auto chunk = reinterpret_cast<qChunk*>(reinterpret_cast<uptr>(data) + offset);
			auto resource = chunk->GetData();

			if (auto inventory = resourceWarehouse->GetInventory(resource->mTypeUID)) {
				inventory->Remove(resource);
			}

			offset += qAlignUp<u32>(chunk->mDataSize, 4) + sizeof(qChunk);
		}

		auto memory_pool = loaded_file->mpPool;
		if (!memory_pool) {
			memory_pool = gMainMemoryPool;
		}

		memory_pool->Free(data);
		
		loaded_file->mData = 0;
		loaded_file->mDataSize = 0;
	}

	int StreamResourceLoader::LoadInternal(LoadedFile* loaded_file)
	{
		UnloadInternal(loaded_file);

		const char* fileName = loaded_file->mFilename;
		s64 loaded_size = 0;

		if (auto buffer = StreamFileWrapper::ReadEntireFile(fileName, &loaded_size, loaded_file->mpPool, loaded_file->mAllocFlags, fileName))
		{
			loaded_file->mData = buffer;
			loaded_file->mDataSize = static_cast<u32>(loaded_size);

			Load(loaded_file->mData, loaded_file->mDataSize, fileName, StreamerMetrics::DATA_UNKNOWN, loaded_file->mfnChunkFilter);

			loaded_file->meState = LoadedFile::STATE_LOADED;
			return 1;
		}


		qPrintf("WARNING: StreamResourceLoader::Load() failed, file_name[%s] since is doesn't exist or has a fileSize of zero\n", fileName);
		return 0;
	}

#endif
}