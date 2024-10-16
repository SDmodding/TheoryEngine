#pragma once

#ifdef THEORY_IMPL
namespace UFG
{
	qMutex gChunkFileBuildersMutex;
	qList<qChunkFileBuilder> gChunkFileBuilders;

	void qValidateChunkFileBuilders()
	{
		qMutexScopeLocker sl(gChunkFileBuildersMutex);
		for (auto i = gChunkFileBuilders.begin(); i != gChunkFileBuilders.end(); i = i->next()) {
			qPrintf("ERROR:  filename='%s'\n", i->mBinFilename.mData);
		}
	}

	qChunkFileBuilder::qChunkFileBuilder(s64 write_buffer_size)
	{
		if (write_buffer_size == -1) {
			write_buffer_size = 0x80000;
		}
		else {
			write_buffer_size = qMax(write_buffer_size, 1024);
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

		qAssert(mWriteBufferPtr != NULL);
	}

	qChunkFileBuilder::~qChunkFileBuilder()
	{
		if (mWriteBufferPtr)
		{
			qFree(mWriteBufferPtr); 
			mWriteBufferPtr = 0;
			mWriteBufferSizeBytes = 0;
			mWriteBufferEOFPos = 0;
		}
	}

	bool qChunkFileBuilder::CreateBuilder(const char* platform, const char* filename, const char* index_filename, bool create_logs, BuildType build_type)
	{
		/* Setup platform */
		{
			if (!qStringCompareInsensitive(platform, "PC")) {
				mTargetEndian = TARGET_ENDIAN_LITTLE;
			}
			else if (!qStringCompareInsensitive(platform, "PS3")) {
				mTargetEndian = TARGET_ENDIAN_BIG;
			}
			else if (!qStringCompareInsensitive(platform, "X360")) {
				mTargetEndian = TARGET_ENDIAN_BIG;
			}
			else if (!qStringCompareInsensitive(platform, "PC64")) {
				mTargetEndian = TARGET_ENDIAN_LITTLE;
			}
			else if (!qStringCompareInsensitive(platform, "Durango")) {
				mTargetEndian = TARGET_ENDIAN_LITTLE;
			}
			else if (!qStringCompareInsensitive(platform, "Orbis")) {
				mTargetEndian = TARGET_ENDIAN_LITTLE;
			}
			else
			{
				qAssertF(false, "ERROR: Unsupported platform '%s'", platform);
				return false;
			}

			mPlatformName.Set(platform);
		}
		
		mBinFilename.Set(filename);
		mInitialBinFileSize = qGetFileSize(mBinFilename);

		mModifiedChunkTally = 0;
		mPreservedChunkTally = 0;
		mStaleChunkTally = 0;
		mRemovedChunkTally = 0;
		mOverwrittenChunkTally = 0;
		mOutOfOrderChunkTally = 0;
		mAddedChunkTally = 0;
		mCreatedChunkTally = 0;
		mUnexpectedChunkTally = 0;

		mCopiedPreCompressedData = false;

		if (index_filename) {
			mIndexFilename.Set(index_filename);
		}
		else {
			mIndexFilename = mBinFilename.ReplaceExtension(".idx");
		}

		mPipelineIndexFilename = mIndexFilename.ReplaceExtension(".pcidx");

		mRecursivelyBuildingAnIndex = (mIndexFilename == mBinFilename);

		mBuildType = build_type;

		/* Change build type to create when one of the file doesn't exist or is recursively building an index. */
		if (mBuildType == BUILDTYPE_INCREMENTAL && (mRecursivelyBuildingAnIndex || !qFileExists(mBinFilename) || !qFileExists(mIndexFilename) || !qFileExists(mPipelineIndexFilename))) {
			mBuildType = BUILDTYPE_CREATE;
		}

		if (mBuildType == BUILDTYPE_CREATE)
		{
			if (!mRecursivelyBuildingAnIndex)
			{
				qDeleteFile(mIndexFilename);
				qDeleteFile(mPipelineIndexFilename);
			}

			if (!index_filename)
			{
				mIndexFilename.SetEmpty();
				mPipelineIndexFilename.SetEmpty();
			}

			mFile = qOpen(filename, QACCESS_WRITE, true);
			qSeek(mFile, 0, QSEEK_SET);
		}

		mWriteInitialPos = 0;
		mWriteCommittedPos = 0;
		mWriteCurrentPos = 0;

		if (create_logs)
		{
			mLogFilename.Format("%s.log.xml", filename);
			mLogFile = qOpen(mLogFilename, QACCESS_WRITE, true);
			mLogIndent.Set("\t");
			mLogIsEnabled = true;

			/* TODO: Use date function after it's implemented. */
			qString date = {};
			qFPrintf(mLogFile, "<File name=\"%s\" date=\"%s\">\n\n", filename, date.mData);
		}
		else
		{
			mLogFile = nullptr;
			mLogIsEnabled = false;
		}

		if (mBuildType == BUILDTYPE_INCREMENTAL && UFG::qFileExists(mPipelineIndexFilename) && UFG::qFileExists(mBinFilename))
		{
			/* TODO: Implement this. */
		}

		if (mBuildType != BUILDTYPE_CREATE) {
			mCurrentOrderedChunk = mOrderedChunks.begin();
		}
		else
		{
			mOrderedChunks.DeleteNodes();
			/* TODO: Clear 'mTreeFileIndex'. */
			mCurrentOrderedChunk = nullptr;
		}

		if (!mFile && mBuildType == BUILDTYPE_CREATE) {
			return false;
		}

		qMutexScopeLocker sl(gChunkFileBuildersMutex);
		gChunkFileBuilders.Insert(this);
		
		return true;
	}

	/* NOTE:
	*	- This is not 100% implementation, so should be re-checked and implemented correctly later on... */
	void qChunkFileBuilder::CloseBuilder(bool* builded, bool verbose)
	{
		/* Remove from builders list */
		{
			qMutexScopeLocker sl(gChunkFileBuildersMutex);
			gChunkFileBuilders.Remove(this);
		}

		qAssertF(mTreeOffset.IsEmpty(), "Offset tree is not empty, some offsets were not resolved by placing a marker\n");

		if (mFileWasWritten) {
			BufferCommit();
		}

		if (mBuildType == BUILDTYPE_INCREMENTAL)
		{
			/* Too much junk before this, complete the implementation... */

			if (!mRecursivelyBuildingAnIndex)
			{
				if (verbose)
				{
					qPrintf("Stats for %s:\n", mBinFilename.mData);
					qPrintf("  mModifiedChunkTally:    %5d\n", mModifiedChunkTally);
					qPrintf("  mPreservedChunkTally:   %5d\n", mPreservedChunkTally);
					qPrintf("  mRemovedChunkTally:     %5d\n", mRemovedChunkTally);
					qPrintf("  mCreatedChunkTally:     %5d\n", mCreatedChunkTally);
					qPrintf("  mAddedChunkTally:       %5d\n", mAddedChunkTally);
					qPrintf("  mOverwrittenChunkTally: %5d\n", mOverwrittenChunkTally);
					qPrintf("  mStaleChunkTally:       %5d\n", mStaleChunkTally);
					qPrintf("  mOutOfOrderChunkTally:  %5d\n", mOutOfOrderChunkTally);
					qPrintf("  mUnexpectedChunkTally:  %5d\n", mUnexpectedChunkTally);
				}

				if (mLogFile && mLogIsEnabled)
				{
					qFPrintf(mLogFile, "Stats for %s:\n", mBinFilename.mData);
					qFPrintf(mLogFile, "  mModifiedChunkTally:    %5d\n", mModifiedChunkTally);
					qFPrintf(mLogFile, "  mPreservedChunkTally:   %5d\n", mPreservedChunkTally);
					qFPrintf(mLogFile, "  mRemovedChunkTally:     %5d\n", mRemovedChunkTally);
					qFPrintf(mLogFile, "  mCreatedChunkTally:     %5d\n", mCreatedChunkTally);
					qFPrintf(mLogFile, "  mAddedChunkTally:       %5d\n", mAddedChunkTally);
					qFPrintf(mLogFile, "  mOverwrittenChunkTally: %5d\n", mOverwrittenChunkTally);
					qFPrintf(mLogFile, "  mStaleChunkTally:       %5d\n", mStaleChunkTally);
					qFPrintf(mLogFile, "  mOutOfOrderChunkTally:  %5d\n", mOutOfOrderChunkTally);
					qFPrintf(mLogFile, "  mUnexpectedChunkTally:  %5d\n", mUnexpectedChunkTally);
				}
			}
		}

		if (mFile)
		{
			if (!mIndexFilename.IsEmpty())
			{
				/* TODO: Implement this... */
			}

			BufferCommit();

			s64 left_space = (qGetFileSize(mFile) - qGetPosition(mFile));
			if (left_space > 0)
			{
				/* TODO:
				*	- There is another chunk inserted 'Chunk.Cruft.Buffer' with uid '0xB1DD1EED'.
				*	- Aligned by 16 bytes and writes just padding.
				*/
			}

			qClose(mFile);
			mFile = nullptr;

			if (builded) {
				*builded = false;
			}
		}
		else if (builded) {
			*builded = false;
		}

		Validate((mBuildType == BUILDTYPE_INCREMENTAL ? "AFTER incremental update" : "AFTER full rebuild"), true);

		if (mLogFile && mLogIsEnabled) 
		{
			qFPrintf(mLogFile, "</File>\n\n");
			qClose(mLogFile);
			mLogFile = nullptr;
			mLogIsEnabled = false;
		}

		if (!mTreeOffset.IsEmpty())
		{
			qPrintf("ERROR: There are %d unresolved offsets for file='%s'\n", mTreeOffset.Count(), mBinFilename.mData);

			/* TODO: Print all unresoled offset name uids:
			*  - "ERROR:   %3d - uid 0x%08x64 at position 0x%08x64 with name '%s'"
			*/
		}
	}

	bool qChunkFileBuilder::Validate(const char* name, bool assertOnError)
	{
		if (!mEnableValidation) {
			return true;
		}

		bool is_valid = true;

		/* TODO: Implement this... 
		*  NOTE:
		*	- I didn't really read the whole thing what it could be doing, but here is brief explainment:
		*	- 1st step:
		*		- Get file size of 'mBinFilename'.
		*		- Looping through each qChunk inside of the file.
		*		- Check for 'gPlatformEndian' with 'mTargetEndian' and swap endianess of each member of 'qChunk' if doesn't match.
		*		- There is then related stuff to 'assertOnError':
		*			- If false: it break out of loop when next chunk is above fileSize.
		*			- If true: it break out of loop when next chunk is equal to fileSize if it's above it assert:
		*				- Assert msg: "Corruption detected %s. Chunk #%d with size 0x%08x (+0x%02x) located at offset 0x%x64 points off the end of the 0x%x64 byte file!"
		*	- 2nd step:
		*		- Seems to be identical to the 1step with exception doing something with 'mTreeFileIndex'.
		*/

		if (is_valid)
		{
			qPrintf("No problems detected in chunkfile %s.\n", name);
			if (mLogFile && mLogIsEnabled) {
				qFPrintf(mLogFile, "No problems detected in chunkfile %s.\n", name);
			}
		}
		else
		{
			qPrintf("ERROR: Corruption detected in chunkfile %s.\n", name);
			if (mLogFile && mLogIsEnabled) {
				qFPrintf(mLogFile, "ERROR: Corruption detected in chunkfile %s.\n", name);
			}
		}

		return is_valid;
	}

	void qChunkFileBuilder::OpenIncremental()
	{
		if (mFile) {
			return;
		}

		qAssert(mBuildType != BUILDTYPE_CREATE);

		if (mBuildType != BUILDTYPE_INCREMENTAL) {
			return;
		}

		if (mEnableValidation)
		{
			qDeleteFile(mBinFilename.ReplaceExtension(".bin.backup"));
			qCopyFile(mBinFilename, mBinFilename.ReplaceExtension(".bin.backup"));

			qDeleteFile(mBinFilename.ReplaceExtension(".idx.backup"));
			qCopyFile(mBinFilename.ReplaceExtension(".idx"), mBinFilename.ReplaceExtension(".idx.backup"));

			qDeleteFile(mBinFilename.ReplaceExtension(".pcidx.backup"));
			qCopyFile(mBinFilename.ReplaceExtension(".pcidx"), mBinFilename.ReplaceExtension(".pcidx.backup"));
		}
		else
		{
			qDeleteFile(mBinFilename.ReplaceExtension(".idx"));
			qDeleteFile(mBinFilename.ReplaceExtension(".pcidx"));
		}

		mFile = qOpen(mBinFilename, QACCESS_APPEND, true);
		qSeek(mFile, mWriteCommittedPos, QSEEK_SET);
	}

	void qChunkFileBuilder::BufferCommit()
	{
		if (mCompressionFile)
		{
			qFlush(mCompressionFile);
			return;
		}

		if (mWriteBufferEOFPos)
		{
			OpenIncremental();

			s64 bytes_written = qWrite(mFile, mWriteBufferPtr, mWriteBufferEOFPos, mWriteCommittedPos, QSEEK_SET);

			qAssertF(bytes_written == mWriteBufferEOFPos, "Failed to write out the required number of bytes. 0x%x64 != 0x%x64", bytes_written, mWriteBufferEOFPos);

			mWriteCommittedPos += mWriteBufferEOFPos;
			mWriteBufferEOFPos = 0;
		}

		if (mWriteCurrentPos != mWriteCommittedPos)
		{
			mWriteCommittedPos = mWriteCurrentPos;
			if (mFile) {
				qSeek(mFile, mWriteCurrentPos, QSEEK_SET);
			}
		}

		qAssert((!mFile || (qGetPosition(mFile) == mWriteCommittedPos)) && (mWriteCommittedPos == mWriteCurrentPos));
	}

	void qChunkFileBuilder::Write(const void* buffer, u32 num_bytes)
	{
		if (!num_bytes) {
			return;
		}

		mFileWasWritten = true;

		/* TODO: Fix this, same related thing as in WriteValue under 'mEnableDebugOutputWrites' if check. */
		if (false)
		{
			qWrite(mCompressionFile, buffer, num_bytes);
			return;
		}

		if (mBufferWrites)
		{
			char* writebuffer;
			while (1)
			{
				s64 write_offset = mWriteCurrentPos - mWriteCommittedPos;
				s64 write_size = mWriteBufferSizeBytes - write_offset;
				writebuffer = &reinterpret_cast<char*>(mWriteBufferPtr)[write_offset];

				if (num_bytes <= write_size) {
					break;
				}

				qMemCopy(writebuffer, buffer, write_size);

				mWriteBufferEOFPos += write_size;
				mWriteCurrentPos += write_size;

				buffer = &reinterpret_cast<const char*>(buffer)[write_size];
				num_bytes -= static_cast<u32>(write_size);

				BufferCommit();

				if (num_bytes <= 0) {
					return;
				}
			}

			qMemCopy(writebuffer, buffer, num_bytes);

			mWriteCurrentPos += num_bytes;
			mWriteBufferEOFPos = qMax(mWriteBufferEOFPos, (mWriteCurrentPos - mWriteCommittedPos));
			return;
		}

		qAssert(mWriteBufferEOFPos == 0);

		OpenIncremental();

		s64 bytes_written = qWrite(mFile, buffer, num_bytes);

		qAssertF(bytes_written == (s64)num_bytes, "Failed to write out the required number of bytes. 0x%x64 != 0x%x64", bytes_written, static_cast<s64>(num_bytes));

		mWriteCurrentPos += bytes_written;
		mWriteCommittedPos = qMax(mWriteCommittedPos, mWriteCurrentPos);

		if (mWriteCurrentPos == mWriteCommittedPos) {
			mBufferWrites = true;
		}
	}

	void qChunkFileBuilder::WriteValue(const void* buffer, u32 num_bytes, const char* name, const char* type_name, const char* value)
	{
		if (mLogFile && mLogIsEnabled)
		{
			qFPrintf(mLogFile, "%s<Value", mLogIndent.mData);

			if (type_name) {
				qFPrintf(mLogFile, " type = \"%s\"", type_name);
			}

			if (name) {
				qFPrintf(mLogFile, "\tname = \"%s\"", name);
			}

			if (value) {
				qFPrintf(mLogFile, ">%s</Value>\n");
			}
			else {
				qFPrintf(mLogFile, "\\>\n");
			}
		}

		if (mEnableDebugOutputWrites)
		{
			/* TODO:
			*	- There should be if check here related to mCompressionFile.
			*	- When if check pass should call 'qGetPosition(mCompressionFile);'
			*/

			int writePos = static_cast<int>(mWriteCurrentPos);
			qPrintf("CFB write: %10.10s %20.20s %08d %08d\n", type_name, name, num_bytes, writePos);
		}

		if (gPlatformEndian == mTargetEndian) {
			return Write(buffer, num_bytes);
		}

		for (const char* p = &reinterpret_cast<const char*>(buffer)[num_bytes - 1]; num_bytes; --num_bytes) {
			Write(p--, 1);
		}
	}
}
#endif