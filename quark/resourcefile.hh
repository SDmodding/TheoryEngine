#pragma once
/*
*	[NOTE] qChunkFileBuilder:
*	- These implementations are based on 'TextureScriberPC64.exe' that was shipped with Triad Wars.
*	- There is no PDB (Debug Symbols) so every function name here is just guessed imagination based on RE.
*	- qChunkFileBuilder class members are still same from sdhdship.pdb as in the TextureScriber.
*/

namespace Illusion
{
	class MemImageSchema;
	class Model;
}

namespace UFG
{
	struct tFileIndex;

	struct tCheckSize : qNode<tCheckSize>
	{
		u64 mPosition;
		u32 mSize;

		tCheckSize(u64 position, u32 size) : mPosition(position), mSize(size) {}
	};

	struct tChunk : qNode<tChunk>
	{
		qString mName;
		u32 mUID;
		u32 mDataOffset;
		int mIsParent;
		u64 mChunkPosition;
		u64 mDataPosition;

		tChunk(const char* name, u32 uid, int is_parent, u32 data_offset, u64 chunk_position, u64 data_position) : mName(name), 
			mUID(uid), mDataOffset(data_offset), mIsParent(is_parent), mChunkPosition(chunk_position), mDataPosition(data_position) {}
	};

	struct tChunkPosition : qNode<tChunkPosition>
	{
		u32 mUID;
		u64 mPosition;
		u32 mByteSize;
	};

	struct tPatchPoint : qNodeRB<tPatchPoint>
	{
		qString mName;
		u64 mPosition;
		u32 mByteSize;
	};

	struct tMarker : qNodeRB64<tMarker>
	{
		u64 mPosition;
		qString mName;
	};

	struct tOffset : qNodeRB64<tOffset>
	{
		qString mName;
		u64 mPosition;
	};

	//-------------------------------------------------------------------
	// File Builder
	//-------------------------------------------------------------------

	class qChunkFileBuilder : public qNode<qChunkFileBuilder>
	{
	public:
		enum BuildType
		{
			BUILDTYPE_CREATE,
			BUILDTYPE_APPEND,
			BUILDTYPE_INCREMENTAL
		};

		enum ChunkStatus
		{
			CHUNK_OPENED_FOR_MODIFY = -4,
			CHUNK_OPENED_FOR_REORDER,
			CHUNK_OPENED_FOR_CREATE,
			CHUNK_OPENED_FOR_SKIP,
			CHUNK_UNUSED,
			CHUNK_PRESERVED,
			CHUNK_CREATED,
			CHUNK_MODIFIED,
			CHUNK_STALE
		};

		enum TargetEndian : s32
		{
			TARGET_ENDIAN_LITTLE,
			TARGET_ENDIAN_BIG
		};

		qString mPlatformName;
		qString mBinFilename;
		qString mIndexFilename;
		qString mPipelineIndexFilename;
		qString mLogFilename;
		TargetEndian mTargetEndian;
		qFile* mFile;
		qFile* mIndexFile;
		qFile* mLogFile;
		BuildType mBuildType;
		bool mFileWasWritten;
		bool mLogIsEnabled;
		bool mGloballyCompressIfPossible;
		qString mLogIndent;
		qString mCompressionFilename;
		qFile* mCompressionFile;
		qFile* mBaseFile;
		qList<tChunk> mChunks;
		qList<tCheckSize> mCheckSizes;
		qTreeRB64<tMarker> mTreeMarker;
		qTreeRB64<tOffset> mTreeOffset;
		qTreeRB64<tMarker> mTreeForbiddenMarkers;
		qTreeRB<tFileIndex> mTreeFileIndex;
		qTreeRB<tPatchPoint> mTreePatchPoints;
		tFileIndex* mCurrentFileIndex;
		qList<tChunkPosition> mOrderedChunks;
		tChunkPosition* mCurrentOrderedChunk;
		bool mBufferWrites;
		void* mWriteBufferPtr;
		s64 mWriteBufferSizeBytes;
		s64 mWriteBufferEOFPos;
		s64 mWriteCommittedPos;
		s64 mWriteCurrentPos;
		s64 mWriteInitialPos;
		s64 mInitialBinFileSize;
		u32 mModifiedChunkTally;
		u32 mPreservedChunkTally;
		u32 mStaleChunkTally;
		u32 mRemovedChunkTally;
		u32 mOverwrittenChunkTally;
		u32 mOutOfOrderChunkTally;
		u32 mAddedChunkTally;
		u32 mCreatedChunkTally;
		u32 mUnexpectedChunkTally;
		bool mRecursivelyBuildingAnIndex;
		bool mCopiedPreCompressedData;
		bool mEnableValidation;
		bool mEnableDebugOutputWrites;

		qChunkFileBuilder(s64 write_buffer_size = -1);

		~qChunkFileBuilder();

		bool CreateBuilder(const char* platform, const char* filename, const char* index_filename = 0, bool create_logs = false, BuildType build_type = BUILDTYPE_CREATE);

		void CloseBuilder(bool* builded, bool verbose);

		bool Validate(const char* name, bool assertOnError);

		void OpenIncremental();

		void BufferCommit();

		void Seek(s64 seek_offset);

		void Write(const void* buffer, u32 num_bytes);

		void WriteValue(const void* buffer, u32 num_bytes, const char* name = 0, const char* type_name = 0, const char* value = 0);

		void WriteRaw(const void* buffer, u32 num_bytes, const char* name = 0, const char* type_name = 0, const char* value = 0);

		void WritePointer(const char* name = 0);

		template <typename T>
		THEORY_INLINE void WriteValue(const T* buffer, const char* name, const char* fmt, const char* type_name)
		{
			char value[256] = { 0 };
			if (mLogFile && mLogIsEnabled) {
				qSPrintf(value, 256, fmt, *buffer);
			}
			WriteValue(buffer, sizeof(T), name, type_name, value);
		}

		void WriteI8(const u8* buffer, const char* name = 0) { WriteValue(buffer, name, "%d", "int8"); }

		void WriteU8(const u8* buffer, const char* name = 0) { WriteValue(buffer, name, "%u", "uint8"); }

		void WriteI16(const i16* buffer, const char* name = 0) { WriteValue(buffer, name, "%d", "int16"); }

		void WriteU16(const i16* buffer, const char* name = 0) { WriteValue(buffer, name, "%u", "uint16"); }

		void WriteI32(const i32* buffer, const char* name = 0) { WriteValue(buffer, name, "%d", "int32"); }

		void WriteU32(const u32* buffer, const char* name = 0) { WriteValue(buffer, name, "%u", "uint32"); }

		void WriteH32(const u32* buffer, const char* name = 0) { WriteValue(buffer, name, "0x%x", "uint32"); }

		void WriteI64(const i64* buffer, const char* name = 0) { WriteValue(buffer, name, "%lld", "int64"); }

		void WriteU64(const u64* buffer, const char* name = 0) { WriteValue(buffer, name, "%llu", "uint64"); }

		void WriteF32(const f32* buffer, const char* name = 0) { WriteValue(buffer, name, "%.6f", "float"); }

		void WriteF64(const f64* buffer, const char* name = 0) { WriteValue(buffer, name, "%.6f", "double"); }

		void Align(u32 align);

		void Padding(u8 byte, u32 size);

		void BeginChunk(u32 uid, const char* name, u32 alignment = 16);

		void EndChunk(u32 uid);

		void BeginCheckSize(u32 size, const char* name = 0);

		bool EndCheckSize();

		bool IsUsingCompressionFile();

		void PushLogIndent();

		void PopLogIndent();

		void LogComment(const char* text, bool new_line = true);

		THEORY_INLINE s64 GetFilePos() { return (IsUsingCompressionFile() ? qGetPosition(mCompressionFile) : mWriteCurrentPos); }

		/* Class Builders */

		template <typename T>
		THEORY_INLINE void Write(const qOffset<T>& offset, const char* name) { WriteI64(&offset.mOffset, qString("%s - offset", name)); }

		void Write(const qBaseNodeRB& node);

		template <typename T, typename U>
		void Write(const qNode<T, U>& node);

		template <typename T, typename U>
		void Write(const qList<T, U>& list);

		void Write(const qResourceHandle& resource_handle, const char* name);

		void Write(const qResourceData& resource_data, const char* name);

		/* Class Illusion Builders */

		void Write(Illusion::MemImageSchema* schema, const Illusion::Model& model);
	};

	inline qChunkFileBuilder::TargetEndian gPlatformEndian;

	void qValidateChunkFileBuilders();

	//-------------------------------------------------------------------

	struct tFileIndex : qNodeRB<tFileIndex>
	{
		qString mFilename;
		u64 mDataPosition;
		u64 mChunkPosition;
		u32 mByteSize;
		u32 mUncompressedSize;
		u32 mDecompressionPadding;
		u64 mCookie;
		qChunkFileBuilder::ChunkStatus mStatus;
	};
}