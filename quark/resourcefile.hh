#pragma once

namespace UFG
{
	struct tFileIndex;

	struct tCheckSize : qNode<tCheckSize>
	{
		u64 mPosition;
		u32 mSize;
	};

	struct tChunk : qNode<tChunk>
	{
		qString mName;
		u32 mUID;
		u32 mDataOffset;
		int mIsParent;
		u64 mChunkPosition;
		u64 mDataPosition;
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
	};

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

#ifdef THEORY_IMPL

	qChunkFileBuilder::qChunkFileBuilder(s64 write_buffer_size)
	{
		if (write_buffer_size == -1) {
			write_buffer_size = 0x80000;
		}
		else if (write_buffer_size < 1024) {
			write_buffer_size = 1024;
		}

		mTargetEndian = TARGET_ENDIAN_LITTLE;

		mFile = mIndexFile = mLogFile = nullptr;

		mLogIndent.Set("\t");

		mFileWasWritten = false;
		mLogIsEnabled = false;
		mGloballyCompressIfPossible = false;

		mCompressionFile = mBaseFile = nullptr;

		mBufferWrites = true;
		mWriteBufferPtr = qMalloc(static_cast<usize>(write_buffer_size), "qChunkFileBuilder::WriteBuffer");
		mWriteBufferSizeBytes = write_buffer_size;

		mWriteBufferEOFPos = 0;
		mWriteCommittedPos = 0;
		mWriteCurrentPos = 0;

		mEnableValidation = false;
		mEnableDebugOutputWrites = false;
	}

#endif
}