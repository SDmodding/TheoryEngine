#pragma once

#ifdef THEORY_IMPL
namespace UFG
{
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