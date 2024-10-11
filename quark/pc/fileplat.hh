#pragma once

namespace UFG
{
	class PCFileDevice : public qFileDevice
	{
	public:
		qFileIoThread mIoThread;
		qFileOpList mFileOpList;

		PCFileDevice() : qFileDevice("pc_hdd"), mIoThread("pc_hdd"), mFileOpList("pc_hdd") {}

		virtual ~PCFileDevice() {}

		virtual void Mount();
		virtual void Unmount();
		virtual void QueueFileOp(qFileOp* file_op, int priority);
		virtual s64 GetFreeDiskSpace(const char* base_directory);
		virtual s64 FindFilesAndDirs(qList<qString>* files, const char* initial_directory, bool recursive, qList<qString>* file_patterns, qList<qString>* dir_patterns);
		virtual bool DirectoryExists(const char* directory);
		virtual bool CreateDirectoryA(const char* directory);
		virtual bool DeleteDirectory(const char* directory);
		virtual bool FilenameExists(const char* filename);
		virtual s64 GetFilenameSize(const char* filenam);
		virtual u64 GetFilenameTime(const char* filename);
		virtual bool SetFilenameTime(const char* filename, u64 modification_time);
		virtual bool GetFilenameReadOnly(const char* filename);
		virtual bool SetFilenameReadOnly(const char* filename, bool make_read_only);
		virtual bool DeleteFilename(const char* filename);
		virtual bool RenameFilename(const char* src_filename, const char* dest_filename);
		virtual bool FileOpen(qFile* file, bool warn_if_fail);
		virtual bool FileClose(qFile* file);
		virtual s64 GetFilePosition(qFile* file);
		virtual s64 GetFileSize(qFile* file);

		virtual s64 FileGetDiskPosSortKey(qFile* file) { return -1; }

		virtual bool FileFlush(qFile* file);
		virtual bool FileSeek(qFile* file, qFileSeekType type, s64 offset);
		virtual s64 FileRead(qFile* file, void* buffer, s64 num_bytes);
		virtual s64 FileWrite(qFile* file, const void* buffer, s64 num_bytes, bool* not_enough_space);
	};

#ifdef THEORY_IMPL

	PCFileDevice gPCFileDevice;

	void PCFileDevice::Mount()
	{
		// TODO
	}

	void PCFileDevice::Unmount()
	{
		// TODO
	}

	void PCFileDevice::QueueFileOp(qFileOp* file_op, int priority)
	{
		mFileOpList.Queue(file_op, priority);
	}

	s64 PCFileDevice::GetFreeDiskSpace(const char* base_directory)
	{
		if (!base_directory) {
			base_directory = "C:\\";
		}

		ULARGE_INTEGER freeBytesAvailableToCaller;
		if (!GetDiskFreeSpaceExA(base_directory, &freeBytesAvailableToCaller, 0, 0)) {
			return 0;
		}

		return freeBytesAvailableToCaller.QuadPart;
	}

	s64 PCFileDevice::FindFilesAndDirs(qList<qString>* files, const char* initial_directory, bool recursive, qList<qString>* file_patterns, qList<qString>* dir_patterns)
	{
		if (!files || qStringEmpty(initial_directory)) {
			return 0;
		}

		// TODO

		return 0;
	}

	bool PCFileDevice::DirectoryExists(const char* directory)
	{
		if (qStringEmpty(directory)) {
			return true;
		}

		WIN32_FIND_DATAA wFindData;
		HANDLE hFindData = FindFirstFileA(directory, &wFindData);
		if (hFindData == INVALID_HANDLE_VALUE) {
			return false;
		}

		FindClose(hFindData);
		return (wFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY);
	}

	bool PCFileDevice::CreateDirectoryA(const char* directory)
	{
		if (qStringEmpty(directory)) {
			return 1;
		}

		// TODO

		return false;
	}

	bool PCFileDevice::DeleteDirectory(const char* directory)
	{
		if (qStringEmpty(directory)) {
			return false;
		}

		return (RemoveDirectoryA(directory) || GetLastError() == ERROR_FILE_NOT_FOUND);
	}
	
	bool PCFileDevice::FilenameExists(const char* filename)
	{
		WIN32_FILE_ATTRIBUTE_DATA dummy;
		return (!qStringEmpty(filename) && GetFileAttributesExA(filename, GetFileExInfoStandard, &dummy));
	}

	s64 PCFileDevice::GetFilenameSize(const char* filename)
	{
		if (qStringEmpty(filename)) {
			return 0;
		}

		WIN32_FILE_ATTRIBUTE_DATA fileAttribute;
		if (!GetFileAttributesExA(filename, GetFileExInfoStandard, &fileAttribute)) {
			return 0;
		}

		return static_cast<s64>(fileAttribute.nFileSizeLow) | (static_cast<s64>(fileAttribute.nFileSizeHigh) << 32ll);
	}

	u64 PCFileDevice::GetFilenameTime(const char* filename)
	{
		if (qStringEmpty(filename)) {
			return 0;
		}

		WIN32_FILE_ATTRIBUTE_DATA fileAttribute;
		FILETIME fileLocalTime;

		if (!GetFileAttributesExA(filename, GetFileExInfoStandard, &fileAttribute) || !FileTimeToLocalFileTime(&fileAttribute.ftLastWriteTime, &fileLocalTime)) {
			return 0;
		}

		return *reinterpret_cast<u64*>(&fileLocalTime);
	}

	bool PCFileDevice::SetFilenameTime(const char* filename, u64 modification_time)
	{
		if (qStringEmpty(filename)) {
			return false;
		}

		HANDLE hFile = CreateFileA(filename, FILE_WRITE_ATTRIBUTES | FILE_READ_ATTRIBUTES, 0, 0, OPEN_EXISTING, 0, 0);
		if (GetLastError() == ERROR_FILE_NOT_FOUND) {
			hFile = CreateFileA(filename, FILE_WRITE_ATTRIBUTES | FILE_READ_ATTRIBUTES, 0, 0, CREATE_NEW, 0, 0);
		}

		FILETIME fileTime;
		LocalFileTimeToFileTime(reinterpret_cast<FILETIME*>(&modification_time), &fileTime);

		bool success = SetFileTime(hFile, 0, 0, &fileTime);
		CloseHandle(hFile);

		return success;
	}


	bool PCFileDevice::GetFilenameReadOnly(const char* filename)
	{
		if (qStringEmpty(filename)) {
			return false;
		}

		WIN32_FILE_ATTRIBUTE_DATA fileAttribute;
		if (!GetFileAttributesExA(filename, GetFileExInfoStandard, &fileAttribute)) {
			return 0;
		}

		return (fileAttribute.dwFileAttributes & FILE_ATTRIBUTE_READONLY);
	}


	bool PCFileDevice::SetFilenameReadOnly(const char* filename, bool make_read_only)
	{
		if (qStringEmpty(filename)) {
			return false;
		}

		WIN32_FILE_ATTRIBUTE_DATA fileAttribute;
		if (!GetFileAttributesExA(filename, GetFileExInfoStandard, &fileAttribute)) {
			return 0;
		}

		if (make_read_only) {
			SetFileAttributesA(filename, fileAttribute.dwFileAttributes | FILE_ATTRIBUTE_READONLY);
		}
		else {
			SetFileAttributesA(filename, fileAttribute.dwFileAttributes & ~FILE_ATTRIBUTE_READONLY);
		}

		return true;
	}

	bool PCFileDevice::DeleteFilename(const char* filename)
	{
		if (qStringEmpty(filename)) {
			return false;
		}

		return (DeleteFileA(filename) || GetLastError() == ERROR_FILE_NOT_FOUND);
	}

	bool PCFileDevice::RenameFilename(const char* src_filename, const char* dest_filename)
	{
		return (!qStringEmpty(src_filename) && !qStringEmpty(dest_filename) && MoveFileA(src_filename, dest_filename));
	}

	bool PCFileDevice::FileOpen(qFile* file, bool warn_if_fail)
	{
		DWORD dwDesiredAccess = 0;
		DWORD dwCreationDisposition = 0;
		DWORD dwFlagsAndAttributes = 0;

		if (file->mAccessType & QACCESS_READ_WRITE)
		{
			dwDesiredAccess = (GENERIC_READ | GENERIC_WRITE);
			dwCreationDisposition = CREATE_ALWAYS;
		}
		else if (file->mAccessType & QACCESS_READ)
		{
			dwDesiredAccess = GENERIC_READ;
			dwCreationDisposition = OPEN_EXISTING;
		}
		else if (file->mAccessType & QACCESS_WRITE)
		{
			dwDesiredAccess = GENERIC_WRITE;
			dwCreationDisposition = CREATE_ALWAYS;
		}
		else if (file->mAccessType & QACCESS_APPEND)
		{
			dwDesiredAccess = GENERIC_WRITE;
			dwCreationDisposition = OPEN_ALWAYS;
		}
		else if (file->mAccessType & QACCESS_WRITE_EXISTING)
		{
			dwDesiredAccess = GENERIC_WRITE;
			dwCreationDisposition = OPEN_EXISTING;
		}

		if (file->mAccessType & QACCESS_SEQUENTIAL) {
			dwFlagsAndAttributes = FILE_FLAG_SEQUENTIAL_SCAN;
		}
		else {
			dwFlagsAndAttributes = FILE_ATTRIBUTE_NORMAL;
		}

		file->mFileHandle = CreateFileA(file->mFilename, dwDesiredAccess, 1, 0, dwCreationDisposition, dwFlagsAndAttributes, 0);

		if (file->mAccessType & QACCESS_APPEND) {
			SetFilePointer(file->mFileHandle, 0, 0, FILE_END);
		}

		if (file->mFileHandle != INVALID_HANDLE_VALUE) {
			return true;
		}

		if (warn_if_fail) {
			qPrintf("WARNING: File open error code 0x%08x : %s\n", GetLastError(), file->mFilename);
		}

		file->mFileHandle = nullptr;
		return false;
	}

	bool PCFileDevice::FileClose(qFile* file)
	{
		if (!file->mFileHandle) {
			return false;
		}

		CloseHandle(file->mFileHandle);
		file->mFileHandle = nullptr;
		return true;
	}

	s64 PCFileDevice::GetFilePosition(qFile* file)
	{
		if (!file->mFileHandle) {
			return -1;
		}

		LONG moveHigh = 0;
		DWORD moveLow = SetFilePointer(file->mFileHandle, 0, &moveHigh, FILE_CURRENT);

		return static_cast<s64>(moveLow) | (static_cast<s64>(moveHigh) << 32ll);
	}

	s64 PCFileDevice::GetFileSize(qFile* file)
	{
		if (!file->mFileHandle) {
			return -1;
		}

		DWORD sizeHigh = 0;
		DWORD sizeLow = ::GetFileSize(file->mFileHandle, &sizeHigh);

		return static_cast<s64>(sizeLow) | (static_cast<s64>(sizeHigh) << 32ll);
	}

	bool PCFileDevice::FileFlush(qFile* file)
	{
		if (!file->mFileHandle) {
			return false;
		}

		FlushFileBuffers(file->mFileHandle);
		return true;
	}

	bool PCFileDevice::FileSeek(qFile* file, qFileSeekType type, s64 offset)
	{
		if (!file->mFileHandle) {
			return false;
		}

		if (type != FILE_CURRENT || offset)
		{
			LONG moveHigh = static_cast<LONG>(offset >> 32ll);
			SetFilePointer(file->mFileHandle, static_cast<LONG>(offset), &moveHigh, type);
		}

		return true;
	}

	s64 PCFileDevice::FileRead(qFile* file, void* buffer, s64 num_bytes)
	{
		if (!file->mFileHandle) {
			return 0;
		}

		DWORD dwNumOfBytesRead;
		ReadFile(file->mFileHandle, buffer, static_cast<DWORD>(num_bytes), &dwNumOfBytesRead, 0);

		return static_cast<s64>(dwNumOfBytesRead);
	}

	s64 PCFileDevice::FileWrite(qFile* file, const void* buffer, s64 num_bytes, bool* not_enough_space)
	{
		if (!file->mFileHandle) {
			return 0;
		}

		DWORD dwNumOfBytesWritten = 0;
		BOOL bWrite = WriteFile(file->mFileHandle, buffer, static_cast<DWORD>(num_bytes), &dwNumOfBytesWritten, 0);

		if (not_enough_space) {
			*not_enough_space = (!bWrite && GetLastError() == ERROR_HANDLE_DISK_FULL);
		}

		return dwNumOfBytesWritten;
	}

#endif
}