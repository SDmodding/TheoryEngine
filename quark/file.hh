#pragma once

namespace UFG
{
	class qFile;
	class qFileOp;
	class qFileDevice;
	class qFileSystem;

	//-------------------------------------------------------------------
	// Enums
	//-------------------------------------------------------------------

	enum qFileAccessType : s32
	{
		QACCESS_READ = 1,
		QACCESS_WRITE = 2,
		QACCESS_READ_WRITE = (QACCESS_READ | QACCESS_WRITE),
		QACCESS_APPEND = 4,
		QACCESS_WRITE_EXISTING = 8,
		QACCESS_SEQUENTIAL = 16,
		QACCESS_READ_SEQUENTIAL = (QACCESS_READ | QACCESS_SEQUENTIAL),
		QACCESS_WRITE_SEQUENTIAL = (QACCESS_WRITE | QACCESS_SEQUENTIAL)
	};

	enum qFileSeekType : s32
	{
		QSEEK_SET,
		QSEEK_CUR,
		QSEEK_END
	};

	//-------------------------------------------------------------------
	// File
	//-------------------------------------------------------------------

	class qFile : public qNode<qFile>
	{
	public:
		qFileDevice* mDevice;
		qFileAccessType mAccessType;
		qMutex mFileHandleMutex;
		void* mFileHandle;
		qMutex mStateMutex;
		s32 mOpenState;
		s32 mCloseState;
		int mNumAsyncOps;
		qFileOp* mAsyncCloseOp;
		char mFilename[260];
	};


	//-------------------------------------------------------------------
	// File Operation
	//-------------------------------------------------------------------

	class qFileOp : public qNode<qFileOp>
	{
	public:
		qFileDevice* mDevice;
		s32 mType;
		u8 mStatus[4];
		qString mFilename;
		qFile* mFile;
		void(__fastcall* mCallback)(qFileOp*, void*);
		void* mCallbackParam;
		float mStartTime;
		float mQueuedTime;
		float mExecuteTime;
		float mDoneTime;
		bool mWarnOnFail;
		void* mBuffer;
		qFileSeekType mSeekType;
		s64 mSeekOffset;
		s64 mNumBytes;
		s64 mFileTime;
		s64 mReadOffset;
		s64 mReadBytes;
		s64 mReadDataOffset;
		s64 mScratchBufferSize;
		void* mScratchBuffer;
		bool mInPlace;
		u64 mPreviousChecksum;
		u64 mAllocationParams;
		qString mAllocationName;
		qMemoryPool* mMemoryPool;
		qString mDestinationFilename;
		bool mRecursive;
		qList<qString> mPatterns;

		union
		{
			u64 mResult0;
			s64 mResultNumBytesRead;
			u64 mResultFileTime;
			s64 mResultFileSize;
			u32 mResultChecksum32;
			s64 mResultChecksum64;
		};

		union
		{
			u64 mResult1;
			void* mResultReadEntireFileBuffer;
			s64 mResultNumBytesWritten;
		};

		qList<qString> mResultFiles;
	};

	class qFileOpList : public qNode<qFileOpList>
	{
	public:
		qFileSystem* mFileSystem;
		qEvent mEvent;
		qMutex mMutex;
		qList<qFileOp> mQueuedOps[3];
		qList<qFileOp> mExecutingOps;
		qList<qFileOp> mDoneOps;

		qFileOpList(const char* name) : mFileSystem(0) {}

		void Queue(qFileOp* file_op, int priority);
	};

	class qFileIoThread
	{
	public:
		const char* mName;
		qThread mThread;
		int mThreadStackSize;
		int mThreadPriority;
		int mThreadLogicalCoreID;
		qFileDevice* mDevice;
		qFileOpList* mFileOpList;
		bool mStopRequested;

		qFileIoThread(const char* name) : mName(name), mDevice(0), mFileOpList(0) {}
	};

	//-------------------------------------------------------------------
	// Device
	//-------------------------------------------------------------------

	class qFileDevice : public qNode<qFileDevice>
	{
	public:
		enum Casing : s32
		{
			CASE_INSENSITIVE,
			CASE_SENSITIVE
		};

		qFileSystem* mFileSystem;
		const char* mName;
		u32 mNameHash;
		int mSupportedAccessModes;
		int mMaxOpenFiles;
		Casing mCaseSensitive;
		qList<qFileOpList> mFileOpLists;
		qMutex mMutex;
		qList<qFile> mOpenFiles;
		qList<qFile> mClosedFiles;

		qFileDevice(const char* name, int supported_access_modes = -1, int max_open_files = INT_MAX, Casing casing = CASE_INSENSITIVE) : mFileSystem(0),
			mName(name), mNameHash(qStringHash32(name)), mSupportedAccessModes(supported_access_modes), mMaxOpenFiles(max_open_files), mCaseSensitive(casing) {}

		virtual ~qFileDevice() {}

		virtual void Mount() = 0;
		virtual void Unmount() = 0;
		virtual void QueueFileOp(qFileOp* file_op, int priority) = 0;
		virtual s64 GetFreeDiskSpace(const char* base_directory) = 0;
		virtual s64 FindFilesAndDirs(qList<qString>* files, const char* initial_directory, bool recursive, qList<qString>* file_patterns, qList<qString>* dir_patterns) = 0;
		virtual bool DirectoryExists(const char* directory) = 0;
		virtual bool CreateDirectoryA(const char* directory) = 0;
		virtual bool DeleteDirectory(const char* directory) = 0;
		virtual bool FilenameExists(const char* filename) = 0;
		virtual s64 GetFilenameSize(const char* filenam) = 0;
		virtual u64 GetFilenameTime(const char* filename) = 0;
		virtual bool SetFilenameTime(const char* filename, u64 modification_time) = 0;
		virtual bool GetFilenameReadOnly(const char* filename) = 0;
		virtual bool SetFilenameReadOnly(const char* filename, bool make_read_only) = 0;
		virtual bool DeleteFilename(const char* filename) = 0;
		virtual bool RenameFilename(const char* src_filename, const char* dest_filename) = 0;
		virtual bool FileOpen(qFile* file, bool warn_if_fail) = 0;
		virtual bool FileClose(qFile* file) = 0;
		virtual s64 GetFilePosition(qFile* file) = 0;
		virtual s64 GetFileSize(qFile* file) = 0;
		virtual s64 FileGetDiskPosSortKey(qFile* file) = 0;
		virtual bool FileFlush(qFile* file) = 0;
		virtual bool FileSeek(qFile* file, qFileSeekType type, s64 offset) = 0;
		virtual s64 FileRead(qFile* file, void* buffer, s64 num_bytes) = 0;
		virtual s64 FileWrite(qFile* file, const void* buffer, s64 num_bytes, bool* not_enough_space) = 0;
	};

#ifdef THEORY_IMPL

	void qFileOpList::Queue(qFileOp* file_op, int priority)
	{
		// TODO
	}

#endif
}