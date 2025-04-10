#pragma once

namespace UFG
{
	class StreamFileWrapper
	{
	public:
		enum QUEUE_CLASS {};

		enum READ_OP_STATE
		{
			STATE_QUEUED,
			STATE_EXECUTING,
			STATE_COMPLETE
		};

		class PackedAudioFileData
		{
		public:
			u32 mAkFileID;
			bool mIsBank;
			u32 mReadCount;
			u32 mWwisePriority;

			virtual ~PackedAudioFileData() {}
		};

		class StreamFile : public qNode<StreamFile>
		{
		public:
			enum FILE_TYPE
			{
				FILE_TYPE_UNPACKED,
				FILE_TYPE_BIG,
				FILE_TYPE_AUDIO_PACK
			};

			s64 mSortKey = -1;
			FILE_TYPE mFileType = FILE_TYPE_UNPACKED;
			BIGFileIndex* mBigFile = 0;
			BIGFileIndex::Entry* mpEntry = 0;
			PackedAudioFileData* mpPackedAudioFileData = 0;
			qFile* mpFile = 0;
		};

		struct StreamReadOp : qNode<StreamReadOp>
		{
			s64 mSeekOffset = 0;
			qEnum<qFileSeekType, s16> mSeekType = QSEEK_CUR;
			qEnum<READ_OP_STATE, s16> mState = STATE_QUEUED;
			qEnum<QUEUE_CLASS, s16> mQueueClass = 3;
			StreamFile* mpStreamFile = 0;
			qFileCallback mCallback = 0;
			void* mCallbackParam = 0;
			void* mpBuffer = 0;
			BIGFileSize mFileSize = { 0, 0, 0, 0 };
			u32 mAudioPriority = 0;

			THEORY_SINLINE u32 smTotalReadOpCount = 0;

			StreamReadOp() { ++smTotalReadOpCount; }
			~StreamReadOp() { --smTotalReadOpCount; }
		};

		THEORY_SINLINE qList<StreamFile> smStreamFiles;
		THEORY_SINLINE qList<StreamReadOp> smCompletedReads;
		THEORY_SINLINE qList<StreamFile> smClosedStreamFiles;
		THEORY_SINLINE qMutex smStreamFileMutex;

		static StreamFile* Open(const char* filename, qFileAccessType accessType = QACCESS_READ, bool bWarnIfFail = 0, qFileCallback callback = 0, void* callback_param = 0);
		static void Close(StreamFile* file);

		static void StreamFileWrapperFileCallback(qFileOp* file_op, void* callback_param);
		static s64 ExecuteReadOp(StreamReadOp* readOp);

		static void* ReadEntireFile(const char* filename, s64* loaded_size = 0, qMemoryPool* memory_pool = 0, u32 alloc_flags = 0, const char* dbg_tag = "StreamFileWrapper::ReadEntireFile");

		static void Service(f32 deltaTime);
	};

#ifdef THEORY_IMPL

	StreamFileWrapper::StreamFile* StreamFileWrapper::Open(const char* filename, qFileAccessType accessType, bool bWarnIfFail, qFileCallback callback, void* callback_param)
	{
		BIGFileIndex* bigFile = 0;
		BIGFileIndex::Entry* entry = 0;

		bool insideBIGFile;
		{
			qMutexScopeLocker sl(smStreamFileMutex);
			insideBIGFile = BigFileSystem::GetFileInfoFromBigFile(filename, bigFile, entry);
		}

		if (insideBIGFile)
		{
			auto streamFile = new ("StreamFile") StreamFile;

			streamFile->mFileType = StreamFile::FILE_TYPE_BIG;      
			streamFile->mBigFile = bigFile;
			streamFile->mpEntry = entry;
			streamFile->mpFile = bigFile->mpFile;
			streamFile->mSortKey = bigFile->mSortKey;

			{
				qMutexScopeLocker sl(smStreamFileMutex);
				smStreamFiles.Insert(streamFile);
			}
			return streamFile;
		}

		qFile* file;
		if (callback) {
			file = qOpenAsync(filename, accessType, bWarnIfFail, callback, callback_param);
		}
		else {
			file = qOpen(filename, accessType, bWarnIfFail);
		}

		if (!file) {
			return 0;
		}

		auto streamFile = new ("StreamFile") StreamFile;

		streamFile->mpFile = file;
		streamFile->mSortKey = qGetDiskPosSortKey(file);

		{
			qMutexScopeLocker sl(smStreamFileMutex);
			smStreamFiles.Insert(streamFile);
		}

		return streamFile;
	}

	void StreamFileWrapper::Close(StreamFile* file)
	{
		if (!file->mBigFile)
		{
			qClose(file->mpFile);
			file->mpFile = 0;
		}

		qMutexScopeLocker sl(smStreamFileMutex);
		file->RemoveFromList();
		smClosedStreamFiles.Insert(file);
	}

	void StreamFileWrapper::StreamFileWrapperFileCallback(qFileOp* file_op, void* callback_param)
	{
		auto readOp = reinterpret_cast<StreamReadOp*>(callback_param);
		if (auto callback = readOp->mCallback) {
			callback(file_op, readOp->mCallbackParam);
		}

		qMutexScopeLocker sl(smStreamFileMutex);
		readOp->RemoveFromList();
		smCompletedReads.Insert(readOp);
	}

	s64 StreamFileWrapper::ExecuteReadOp(StreamReadOp* readOp)
	{
		qFileSeekType seek_type;
		s64 seek_offset;
		
		auto streamFile = readOp->mpStreamFile;
		auto file = streamFile->mpFile;
		auto buffer = readOp->mpBuffer;

		if (streamFile->mBigFile && streamFile->mFileType == StreamFile::FILE_TYPE_BIG)
		{
			seek_type = QSEEK_SET;
			seek_offset = static_cast<s64>(streamFile->mpEntry->offset_div_4) * 4;
		}
		else
		{
			if (streamFile->mFileType == StreamFile::FILE_TYPE_AUDIO_PACK) {
				++streamFile->mpPackedAudioFileData->mReadCount;
			}

			seek_type = readOp->mSeekType;
			seek_offset = readOp->mSeekOffset;
		}

		readOp->mState = STATE_EXECUTING;

		const bool noCallback = readOp->mCallback == 0;
		const s64 compressed_size = readOp->mFileSize.compressed_size;
		const s64 uncompressed_size = readOp->mFileSize.uncompressed_size;

		if (!compressed_size || compressed_size == uncompressed_size)
		{
			if (noCallback) {
				return qRead(file, buffer, uncompressed_size, seek_type);
			}

			qReadAsync(file, buffer, uncompressed_size, seek_offset, seek_type, StreamFileWrapperFileCallback, readOp);
			return 0;
		}

		const s64 read_data_offset = readOp->mFileSize.load_offset & 0xFFF;
		const s64 read_offset = readOp->mFileSize.load_offset & 0xFFFFF000;
		const s64 num_bytes = uncompressed_size + readOp->mFileSize.compressed_extra;

		if (noCallback) {
			return qReadAndDecompress(file, buffer, num_bytes, seek_offset, seek_type, read_offset, compressed_size, read_data_offset);
		}
		
		qReadAndDecompressAsync(file, buffer, num_bytes, seek_offset, seek_type, read_offset, compressed_size, read_data_offset, 0, 0, 1, StreamFileWrapperFileCallback, readOp);
		return 0;
	}

	void* StreamFileWrapper::ReadEntireFile(const char* filename, s64* loaded_size, qMemoryPool* memory_pool, u32 alloc_flags, const char* dbg_tag)
	{
		BIGFileSize fileSize;

		BIGFileIndex* bigFile = 0;
		BIGFileIndex::Entry* entry = 0;

		if (BigFileSystem::GetFileInfoFromBigFile(filename, bigFile, entry)) {
			fileSize = entry->size;
		}
		else
		{
			fileSize.compressed_size = static_cast<u32>(qGetFileSize(filename));
			fileSize.load_offset = 0;
			fileSize.uncompressed_size = fileSize.compressed_size;
			fileSize.compressed_extra = 0;
		}

		auto streamFile = Open(filename, QACCESS_READ, 1);
		if (streamFile)
		{
			if (!memory_pool) {
				memory_pool = gMainMemoryPool;
			}

			auto buffer = memory_pool->Allocate(fileSize.uncompressed_size + fileSize.compressed_extra, dbg_tag, alloc_flags, 1);
			if (buffer)
			{
				auto readOp = new ("StreamReadOp") StreamReadOp;
				readOp->mpStreamFile = streamFile;
				readOp->mpBuffer = buffer;
				readOp->mFileSize = fileSize;
				readOp->mSeekType = QSEEK_SET;

				ExecuteReadOp(readOp);
				qDelete(readOp);

				if (loaded_size) {
					*loaded_size = fileSize.uncompressed_size;
				}
			}

			Close(streamFile);
			return buffer;
		}

		return 0;
	}

	void StreamFileWrapper::Service(f32 deltaTime)
	{
		// TODO: Implement missing stuff...

		qMutexScopeLocker sl(smStreamFileMutex);

		smCompletedReads.DeleteNodes();
		smClosedStreamFiles.DeleteNodes();
	}

#endif
}