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

	enum qFileMapType
	{
		FILE_MAP_TYPE_DEFAULT,
		FILE_MAP_TYPE_HOT_SWAP,
		NUM_FILE_MAP_TYPE
	};

	//-------------------------------------------------------------------
	// Mapping
	//-------------------------------------------------------------------

	class qFileMapLocation : public qNodeRB<qFileMapLocation>
	{
	public:
		const char* mMappingName;
		qString mPhysicalLocations[2];
	};

	class qFileMappingCallback : public qNode<qFileMappingCallback>
	{
	public:
		bool(*mCallback)(qFileMapType, const char*, qString*, const void*);
		const void* mParam;
	};

	//-------------------------------------------------------------------
	// File
	//-------------------------------------------------------------------

	class qFile : public qNode<qFile>
	{
	public:
		enum State : s32
		{
			STATE_NONE,
			STATE_OPENING,
			STATE_OPENED,
			STATE_CLOSING,
			STATE_CLOSED
		};

		qFileDevice* mDevice;
		qFileAccessType mAccessType;
		qMutex mFileHandleMutex;
		void* mFileHandle;
		qMutex mStateMutex;
		State mOpenState;
		State mCloseState;
		int mNumAsyncOps;
		qFileOp* mAsyncCloseOp;
		char mFilename[260];

		qFile(qFileDevice* device, const char* filename, qFileAccessType access_type) : mFileHandleMutex("qFile::mOperationMutex"), mStateMutex("qFile::mStateMutex"),
			mDevice(device), mAccessType(access_type), mOpenState(STATE_NONE), mCloseState(STATE_NONE), mNumAsyncOps(0), mAsyncCloseOp(0), mFileHandle(nullptr)
		{
			qStringCopy(mFilename, sizeof(mFilename), filename);
		}
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
		virtual s64 GetFilenameSize(const char* filename) = 0;
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

	//-------------------------------------------------------------------
	// System
	//-------------------------------------------------------------------

	class qFileSystem
	{
	public:
		s64 mStartTicks;
		bool mReportSyncFileOps;
		bool mReportAsyncFileOps;
		qList<qFileDevice> mDevices;
		qMutex mMappingMutex;
		qList<qFileMappingCallback> mMappingCallbacks;
		qTreeRB<qFileMapLocation> mMappingLocations;
		qFileDevice*(*mMapFilenameToDeviceCallback)(const char*);
		qMutex mMutex;
		qEvent mEvent;
		qList<qFileOp> mDoneFileOps;
		bool mFatalIOError;

		qFileSystem() : mReportSyncFileOps(false), mReportAsyncFileOps(false), mFatalIOError(false), mMapFilenameToDeviceCallback(nullptr)
		{
			mEvent.Create("qFileSystem", false);
		}

		~qFileSystem();

		void InitDevice(qFileDevice* device);

		void CloseDevice(qFileDevice* device);

		void InitPlat();

		void ClosePlat();

		bool IsBusy();

		qString MapFilename(qFileMapType file_map_type, const char* filename);

		qFileDevice* MapFilenameToDevice(const char* filename);

		qFileDevice* MapFilenameToDevicePlat(const char* filename);

		void Service();
	};

	extern qFileSystem gQuarkFileSystem;

	//-------------------------------------------------------------------
	// Functions
	//-------------------------------------------------------------------

	qString qGetDirectory();

	//-------------------------------------------------------------------
	// File Functions
	//-------------------------------------------------------------------

	void qFPrintf(qFile* file, const char* fmt, ...);

	qFile* qOpen(const char* filename, qFileAccessType access_type, bool warn_if_fail);

	bool qOpenInternal(qFile* file, bool warn_if_fail);

	void qClose(qFile* file);

	bool qCloseInternal(qFile* file);

	bool qWaitForOpenFileHandle(qFile* file);

	s64 qGetFileSize(qFile* file);

	s64 qGetFileSize(const char* filename);

	s64 qGetPosition(qFile* file);

	bool qSetFileTime(const char* filename, u64 file_time);

	s64 qRead(qFile* file, void* buffer, s64 num_bytes, s64 seek_offset = 0, qFileSeekType seek_type = QSEEK_CUR);

	s64 qRead(const char* filename, void* buffer, s64 num_bytes, s64 seek_position = 0);

	char* qReadEntireFile(const char* filename, s64* loaded_size = nullptr, qMemoryPool* memory_pool = nullptr, u64 allocation_params = 0, const char* name = nullptr);

	s64 qWrite(qFile* file, const void* buffer, s64 num_bytes, s64 seek_offset = 0, qFileSeekType seek_type = QSEEK_CUR, bool* not_enough_space = nullptr);

	s64 qWrite(const char* filename, const void* buffer, s64 num_bytes, s64 seek_offset = 0, qFileSeekType seek_type = QSEEK_CUR, bool* not_enough_space = nullptr);

	s64 qWriteAppend(const char* filename, const void* buffer, s64 num_bytes, s64 seek_offset = 0, qFileSeekType seek_type = QSEEK_CUR, bool* not_enough_space = nullptr);

	bool qWriteString(qFile* file, const char* text, int length = -1);

	bool qFileExists(const char* filename);

	void qDeleteFile(const char* filename);

	bool qCopyFile(const char* src_filename, const char* dest_filename);

	bool qFlush(qFile* file);

	void qSeek(qFile* file, s64 seek_offset, qFileSeekType seek_type);

	void qTouch(const char* filename);

#ifdef THEORY_IMPL

	void qFileOpList::Queue(qFileOp* file_op, int priority)
	{
		/* TODO: Implement this. */
	}

	//-------------------------------------------------------------------
	// System
	//-------------------------------------------------------------------

	qFileSystem gQuarkFileSystem;

	qFileSystem::~qFileSystem()
	{
		do
		{
			Service();
			qSleep(1);
		} while (IsBusy());

		/* TODO: Implement this. */
	}

	void qFileSystem::InitDevice(qFileDevice* device)
	{
		mDevices.Insert(device);

		device->mFileSystem = this;
		device->Mount();
	}

	void qFileSystem::CloseDevice(qFileDevice* device)
	{
		if (!device->mFileSystem) {
			return;
		}

		device->Unmount();
		device->mFileSystem = nullptr;

		mDevices.Remove(device);
	}

	bool qFileSystem::IsBusy()
	{
		bool busy = false;

		for (auto device = mDevices.begin(); device != mDevices.end(); device = device->next())
		{
			if (busy) {
				break;
			}

			qMutexScopeLocker sl(device->mMutex);

			for (auto openFile = device->mOpenFiles.begin(); openFile != device->mOpenFiles.end(); openFile = openFile->next())
			{
				if (openFile->mNumAsyncOps != 0) 
				{
					busy = true;
					break;
				}
			}
		}

		return busy;
	}

	qString qFileSystem::MapFilename(qFileMapType file_map_type, const char* filename)
	{
		/* TODO: Complete the implementation... */
		if (qStringEmpty(filename)) {
			return {};
		}


		qMutexScopeLocker sl(mMappingMutex);

		qString mappingStr;

		if (filename[0] == '$' && filename[1] == '(')
		{
			/* TODO: Implement this. */
		}

		for (auto i = mMappingCallbacks.begin(); i != mMappingCallbacks.end(); i = i->next()) {
			i->mCallback(file_map_type, filename, &mappingStr, i->mParam);
		}


		return filename;
	}

	qFileDevice* qFileSystem::MapFilenameToDevice(const char* filename)
	{
		if (mMapFilenameToDeviceCallback)
		{
			if (auto device = mMapFilenameToDeviceCallback(filename)) {
				return device;
			}
		}

		return MapFilenameToDevicePlat(filename);
	}

	void qFileSystem::Service()
	{
		/* TODO: Implement this. */
	}

	//-------------------------------------------------------------------
	// File Functions
	//-------------------------------------------------------------------

	qFile* qOpen(const char* filename, qFileAccessType access_type, bool warn_if_fail)
	{
		auto mapped_filename = gQuarkFileSystem.MapFilename(FILE_MAP_TYPE_DEFAULT, filename);
		auto device = gQuarkFileSystem.MapFilenameToDevice(filename);

		auto file = new ("qFile") qFile(device, mapped_filename, access_type);
		file->mOpenState = qFile::STATE_OPENING;

		if (!qOpenInternal(file, warn_if_fail))
		{
			qDelete(file);
			file = nullptr;
		}

		return file;
	}

	bool qOpenInternal(qFile* file, bool warn_if_fail)
	{
		auto device = file->mDevice;
		bool can_open = (file->mFilename[0] && device && !gQuarkFileSystem.mFatalIOError);
		bool directory_valid = true;
		bool opened = false;

		if (file->mAccessType & QACCESS_WRITE || file->mAccessType & QACCESS_APPEND || file->mAccessType & QACCESS_WRITE_SEQUENTIAL)
		{
			qString filename = { file->mFilename };
			qString filepath = filename.GetFilePath();
			if (!can_open || !device->CreateDirectoryA(filepath))
			{
				qPrintf("WARNING:  Could not create directory '%s' when opening '%s'.  Current Dir = '%s'\n", filename.mData, file->mFilename, qGetDirectory().mData);
				directory_valid = false;
			}
		}

		if (can_open && directory_valid)
		{
			qMutexScopeLocker sl(file->mFileHandleMutex);
			opened = device->FileOpen(file, warn_if_fail);
		}

		/* Change state */
		{
			qMutexScopeLocker sl(file->mStateMutex);

			if (opened) {
				file->mOpenState = qFile::STATE_OPENED;
			}
			else
			{
				file->mOpenState = qFile::STATE_NONE;
				file->mCloseState = qFile::STATE_CLOSED;
			}
		}

		if (opened)
		{
			qMutexScopeLocker sl(device->mMutex);
			device->mOpenFiles.Insert(file);

			return true;
		}
		
		if (warn_if_fail) {
			qPrintf("ERROR: Unable to open file = '%s'\n", file->mFilename);
		}

		return false;
	}

	void qClose(qFile* file)
	{
		if (!file) {
			return;
		}

		/* Set close state */
		{
			qMutexScopeLocker sl(file->mStateMutex);
			file->mCloseState = qFile::STATE_CLOSING;
		}

		qWaitForOpenFileHandle(file);
		qCloseInternal(file);
		qDelete(file);
	}

	bool qCloseInternal(qFile* file)
	{
		if (!file) {
			return false;
		}

		auto device = file->mDevice;
		bool closing = false;

		/* Check close state */
		{
			qMutexScopeLocker sl(file->mStateMutex);
			closing = (file->mCloseState == qFile::STATE_CLOSING);
		}

		{
			qMutexScopeLocker sl(file->mStateMutex);

			file->mOpenState = qFile::STATE_NONE;
			file->mCloseState = qFile::STATE_CLOSED;
			file->mAsyncCloseOp = 0;
		}

		if (closing && device)
		{
			qMutexScopeLocker sl(file->mFileHandleMutex);
			device->FileClose(file);
		}

		if (device)
		{
			qMutexScopeLocker sl(device->mMutex);
			device->mOpenFiles.Remove(file);
		}

		return true;
	}

	bool qWaitForOpenFileHandle(qFile* file)
	{
		if (gQuarkFileSystem.mFatalIOError || !file || !file->mDevice) {
			return false;
		}

		if (file->mOpenState == qFile::STATE_NONE || file->mOpenState == qFile::STATE_OPENED) {
			return true;
		}

		if (file->mCloseState) {
			return false;
		}

		while (file->mOpenState == qFile::STATE_OPENING) {
			qSleep(0);
		}

		return (file->mOpenState == qFile::STATE_OPENED);
	}

	s64 qGetFileSize(qFile* file)
	{
		if (!qWaitForOpenFileHandle(file)) {
			return -1;
		}

		s64 size = -1;
		{
			qMutexScopeLocker sl(file->mFileHandleMutex);
			size = file->mDevice->GetFileSize(file);
		}

		return size;
	}

	s64 qGetFileSize(const char* filename)
	{
		auto device = gQuarkFileSystem.MapFilenameToDevice(filename);
		auto mapped_filename = gQuarkFileSystem.MapFilename(FILE_MAP_TYPE_DEFAULT, filename);

		if (gQuarkFileSystem.mFatalIOError || !device) {
			return 0;
		}

		return device->GetFilenameSize(mapped_filename);
	}

	s64 qGetPosition(qFile* file)
	{
		s64 position = -1;
		{
			qMutexScopeLocker sl(file->mFileHandleMutex);

			if (file->mOpenState == qFile::STATE_OPENED) {
				position = file->mDevice->GetFilePosition(file);
			}
		}

		return position;
	}


	bool qSetFileTime(const char* filename, u64 file_time)
	{
		auto device = gQuarkFileSystem.MapFilenameToDevice(filename);
		auto mapped_filename = gQuarkFileSystem.MapFilename(FILE_MAP_TYPE_DEFAULT, filename);

		if (gQuarkFileSystem.mFatalIOError || !device) {
			return false;
		}

		return device->SetFilenameTime(mapped_filename, file_time);
	}

	s64 qRead(qFile* file, void* buffer, s64 num_bytes, s64 seek_offset, qFileSeekType seek_type)
	{
		if (!qWaitForOpenFileHandle(file) || !buffer) {
			return -1;
		}

		if (!num_bytes) {
			return 0;
		}

		s64 num_read_bytes;

		{
			qMutexScopeLocker sl(file->mFileHandleMutex);
			num_read_bytes = file->mDevice->FileRead(file, buffer, num_bytes);
		}

		return num_read_bytes;
	}

	s64 qRead(const char* filename, void* buffer, s64 num_bytes, s64 seek_position)
	{
		if (!num_bytes) {
			return 0;
		}

		s64 num_read_bytes = -1;

		if (auto file = qOpen(filename, QACCESS_READ, true))
		{
			num_read_bytes = qRead(file, buffer, num_bytes, seek_position, QSEEK_SET);
			qClose(file);
		}

		return num_read_bytes;
	}

	char* qReadEntireFile(const char* filename, s64* loaded_size, qMemoryPool* memory_pool, u64 allocation_params, const char* name)
	{
		if (gQuarkFileSystem.mFatalIOError || qStringEmpty(filename)) {
			return nullptr;
		}

		s64 size = qGetFileSize(filename);
		if (loaded_size) {
			*loaded_size = size;
		}

		if (size <= 0) {
			return nullptr;
		}

		if (!memory_pool) {
			memory_pool = GetMainMemoryPool();
		}

		auto file = qOpen(filename, QACCESS_READ, true);
		if (!file) {
			return nullptr;
		}

		char alloc_namebuf[384];
		const char* alloc_name = name;
		if (!alloc_name)
		{
			qSPrintf(alloc_namebuf, "qReadEntireFile('%s')", filename);
			alloc_name = alloc_namebuf;
		}

		char* filedata = reinterpret_cast<char*>(memory_pool->Allocate(static_cast<usize>(size + 1), alloc_name, allocation_params, true));
		if (filedata)
		{
			if (qRead(file, filedata, size) == size) {
				filedata[size] = 0;
			}
			else
			{
				memory_pool->Free(filedata);
				filedata = nullptr;
			}
		}

		qClose(file);
		return filedata;
	}

	s64 qWrite(qFile* file, const void* buffer, s64 num_bytes, s64 seek_offset, qFileSeekType seek_type, bool* not_enough_space)
	{
		if (!qWaitForOpenFileHandle(file) || !buffer) {
			return -1;
		}

		if (!num_bytes) {
			return 0;
		}

		bool n_enough_space = false;
		s64 num_written_bytes;

		{
			qMutexScopeLocker sl(file->mFileHandleMutex);
			num_written_bytes = file->mDevice->FileWrite(file, buffer, num_bytes, &n_enough_space);
		}

		if (not_enough_space) {
			*not_enough_space = n_enough_space;
		}

		return num_written_bytes;
	}

	s64 qWrite(const char* filename, const void* buffer, s64 num_bytes, s64 seek_offset, qFileSeekType seek_type, bool* not_enough_space)
	{
		if (not_enough_space) {
			*not_enough_space = false;
		}

		if (!num_bytes) {
			return 0;
		}

		s64 num_written_bytes = -1;

		if (auto file = qOpen(filename, QACCESS_WRITE, true))
		{
			num_written_bytes = qWrite(file, buffer, num_bytes, seek_offset, seek_type, not_enough_space);
			qClose(file);
		}

		return num_written_bytes;
	}

	s64 qWriteAppend(const char* filename, const void* buffer, s64 num_bytes, s64 seek_offset, qFileSeekType seek_type, bool* not_enough_space)
	{
		if (not_enough_space) {
			*not_enough_space = false;
		}

		if (!num_bytes) {
			return 0;
		}

		s64 num_written_bytes = -1;

		if (auto file = qOpen(filename, QACCESS_APPEND, true))
		{
			num_written_bytes = qWrite(file, buffer, num_bytes, seek_offset, seek_type, not_enough_space);
			qClose(file);
		}

		return num_written_bytes;
	}

	bool qWriteString(qFile* file, const char* text, int length)
	{
		s64 num_bytes = static_cast<s64>(length == -1 ? qStringLength(text) : length);

		return qWrite(file, text, num_bytes) > 0;
	}

	bool qFileExists(const char* filename)
	{
		auto device = gQuarkFileSystem.MapFilenameToDevice(filename);
		auto mapped_filename = gQuarkFileSystem.MapFilename(FILE_MAP_TYPE_DEFAULT, filename);

		if (gQuarkFileSystem.mFatalIOError || !device) {
			return false;
		}

		return device->FilenameExists(mapped_filename);
	}

	void qDeleteFile(const char* filename)
	{
		auto device = gQuarkFileSystem.MapFilenameToDevice(filename);
		auto mapped_filename = gQuarkFileSystem.MapFilename(FILE_MAP_TYPE_DEFAULT, filename);

		if (!gQuarkFileSystem.mFatalIOError && device) {
			device->DeleteFilename(mapped_filename);
		}
	}

	bool qCopyFile(const char* src_filename, const char* dest_filename)
	{
		if (qStringEmpty(src_filename) || qStringEmpty(dest_filename) || !qFileExists(src_filename)) {
			return false;
		}

		bool copied = false;

		if (qFile* dest_file = qOpen(dest_filename, QACCESS_WRITE, true))
		{
			if (qFile* src_file = qOpen(src_filename, QACCESS_READ, true))
			{
				s64 size = 0;
				if (qWaitForOpenFileHandle(src_file))
				{
					qMutexScopeLocker sl(src_file->mFileHandleMutex);
					size = src_file->mDevice->GetFileSize(src_file);
				}

				s64 buffer_size = qMin(size, 0x10000);
				if (size > 0)
				{
					if (void* buffer = qMalloc(buffer_size, "qCopyFile buffer", 1))
					{
						while (size > 0)
						{
							s64 rw_size = qMin(size, buffer_size);
							if (qRead(src_file, buffer, rw_size) != rw_size || qWrite(dest_file, buffer, rw_size) != rw_size) {
								break;
							}

							size -= rw_size;
						}

						copied = (size <= 0);

						qFree(buffer);
					}
				}
				else {
					copied = true;
				}

				qClose(src_file);
			}

			qClose(dest_file);
		}

		return copied;
	}

	bool qFlush(qFile* file)
	{
		bool flushed = false;

		if (qWaitForOpenFileHandle(file))
		{
			qMutexScopeLocker sl(file->mFileHandleMutex);
			flushed = file->mDevice->FileFlush(file);
		}

		return flushed;
	}


	void qSeek(qFile* file, s64 seek_offset, qFileSeekType seek_type)
	{
		if (qWaitForOpenFileHandle(file))
		{
			qMutexScopeLocker sl(file->mFileHandleMutex);
			file->mDevice->FileSeek(file, seek_type, seek_offset);
		}
	}

	void qTouch(const char* filename)
	{
		qSetFileTime(filename, qGetSystemTime());
	}

#endif
}