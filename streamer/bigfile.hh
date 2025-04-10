#pragma once

namespace UFG
{
	THEORY_SINLINE u32 gBigFileNextMountIndex;

	struct BIGFileSize
	{
		u32 load_offset;
		u32 compressed_size;
		u32 compressed_extra;
		u32 uncompressed_size;
	};

	class BIGFileIndex : public qResourceData
	{
	public:
		enum State
		{
			STATE_DISABLED,
			STATE_ENABLED,
			STATE_PATCH,
			STATE_PATCH2,
			STATE_OVERRIDING
		};

		struct Entry
		{
			u32 uid;
			u32 offset_div_4;
			BIGFileSize size;
		};

		s64 mSortKey;
		u32 mEntryCount;
		qOffset64<Entry*> mEntries;
		qFile* mpFile;
		u16 mState;
		u16 mMountIndex;
		u32 mPadding0;
		u32 mPadding1;
		char mBigFileName[32];
	};

	class BIGFileInventory : public qResourceInventory
	{
	public:
		BIGFileInventory() : qResourceInventory("BIGFileInventory", RTypeUID_BIGFileIndex, ChunkUID_BIGFileIndex) {}

		void Add(qResourceData* data) override;
		void Remove(qResourceData* data) override;
	} extern gBIGFileInventory;

	class BigFileSystem
	{
	public:
		THEORY_SINLINE qString m_BigFileNamePrefix;

		static qString BigFilePrepend(const qString& fileName) {
			return { "%s%s", m_BigFileNamePrefix.mData, fileName.mData };
		}

		static bool GetFileInfoFromBigFile(const char* filename, BIGFileIndex*& _bigFile, BIGFileIndex::Entry*& _entry);
	};

#ifdef THEORY_IMPL

	//----------------------------
	//	Big File Inventory
	//----------------------------

	BIGFileInventory gBIGFileInventory;

	void BIGFileInventory::Add(qResourceData* data)
	{
		auto bigFileIndex = static_cast<BIGFileIndex*>(data);
		new (bigFileIndex) BIGFileIndex;

		qString fileName = bigFileIndex->mBigFileName;
		fileName = BigFileSystem::BigFilePrepend(fileName + ".big");

		bigFileIndex->mMountIndex = gBigFileNextMountIndex++;

		auto file = qOpen(fileName, QACCESS_READ, 1);
		bigFileIndex->mpFile = file;

		if (file)
		{
			bigFileIndex->mState = BIGFileIndex::STATE_ENABLED;
			bigFileIndex->mSortKey = qGetDiskPosSortKey(file);
		}
		else {
			bigFileIndex->mState = BIGFileIndex::STATE_DISABLED;
		}

		qResourceInventory::Add(data);
	}

	void BIGFileInventory::Remove(qResourceData* data)
	{
		auto bigFileIndex = static_cast<BIGFileIndex*>(data);
		if (auto file = bigFileIndex->mpFile)
		{
			qClose(file);
			bigFileIndex->mpFile = 0;
		}

		qResourceInventory::Remove(data);
	}

	//----------------------------
	//	Big File System
	//----------------------------

	bool BigFileSystem::GetFileInfoFromBigFile(const char* filename, BIGFileIndex*& _bigFile, BIGFileIndex::Entry*& _entry)
	{
		while (static_cast<u8>(*filename) - '.' <= 1 || *filename == '\\') {
			++filename;
		}

		char str[256];
		qStringCopy(str, filename);

		for (char* c = str; *c; ++c)
		{
			if (*c == '/') {
				*c = '\\';
			}
		}

		const u32 uid = qStringHashUpper32(str);

		for (int state = BIGFileIndex::STATE_OVERRIDING; state != BIGFileIndex::STATE_DISABLED; --state)
		{
			for (auto data : gBIGFileInventory.mResourceDatas)
			{
				auto bigFileIndex = static_cast<BIGFileIndex*>(data);
				if (bigFileIndex->mState != state) {
					continue;
				}

				auto entries = bigFileIndex->mEntries.Get();

				int index = 0;
				int maxIndex = bigFileIndex->mEntryCount - 1;
				while (maxIndex >= index)
				{
					int i = (maxIndex + index) / 2;
					auto entry = &entries[i];

					if (uid > entry->uid)
					{
						index = i + 1;
						continue;
					}

					if (uid < entry->uid)
					{
						maxIndex = i - 1;
						continue;
					}

					_bigFile = bigFileIndex;
					_entry = entry;
					return true;
				}
			}
		}

		return false;
	}

#endif
}