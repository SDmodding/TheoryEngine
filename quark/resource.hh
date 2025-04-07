#pragma once

struct MemImageLoadFlag
{
	u32 flag;
};

namespace UFG
{
	class qResourceData;
	class qResourceInventory;

	class qChunk
	{
	public:
		u32 mUID;
		u32 mChunkSize;
		u32 mDataSize;
		u32 mDataOffset;

		qChunk() {}
		qChunk(u32 uid, u32 chunk_size, u32 data_size, u32 data_offset) : mUID(uid), mChunkSize(chunk_size), mDataSize(data_size), mDataOffset(data_offset) {}

		THEORY_INLINE qResourceData* GetData() { return reinterpret_cast<qResourceData*>(reinterpret_cast<uptr>(&this[1]) + mDataOffset); }
	};

	//-------------------------------------------------------------------
	//	Resource Handle
	//-------------------------------------------------------------------

	class qResourceHandle : public qNode<qResourceHandle>
	{
	public:
		qResourceData* mData;
		u32 mNameUID;
		u32 mTailPad;

		qResourceHandle() : mData(0), mNameUID(0) {}

		~qResourceHandle()
		{
			Close();
		}

		void Close();
		void Close(qResourceInventory* inventory);

		void Init(u32 type_uid, u32 name_uid);
		void Init(u32 type_uid, u32 name_uid, qResourceData* resource_data, qResourceInventory* inventory);
		void Init(u32 type_uid, u32 name_uid, qResourceInventory* inventory);

		bool IsDefault();

		template <typename T>
		THEORY_INLINE T GetData() { return reinterpret_cast<T>(mData); }
	};

	class qGenericResourceHandle : public qResourceHandle
	{
	public:
		u32 mTypeUID;

		THEORY_INLINE void Init() { qResourceHandle::Init(mTypeUID, mNameUID); }

		THEORY_INLINE void SetTypeNameUID(u32 type_uid, u32 name_uid)
		{
			mNameUID = name_uid;
			mTypeUID = type_uid;
		}
	};

	template <u32 type_uid, typename T>
	class qTypedResourceHandle : public qResourceHandle
	{
	public:
		enum { ResourceTypeUID = type_uid };

		THEORY_INLINE T* GetData() { return reinterpret_cast<T*>(mData); }
	};

	//-------------------------------------------------------------------
	//	Resource Data
	//-------------------------------------------------------------------

	class qResourceData : public qNodeRB<qResourceData>
	{
	public:
		qList<qResourceHandle> mResourceHandles;
		u32 mTypeUID;
		char mDebugName[36];

		qResourceData() {}

		qResourceData(u32 type_uid, u32 name_uid, const char* name)
		{
			mNode.mUID = name_uid;
			mTypeUID = type_uid;
			SetDebugName(name);
		}

		void SetDebugName(const char* name);
	};

	//-------------------------------------------------------------------
	//	Inventory
	//-------------------------------------------------------------------

	class qResourceInventory : public qNodeRB<qResourceInventory>, public qNode<qResourceInventory>
	{
	public:
		u32 mDefaultResourceNameUID;
		u32 mChunkUID;
		const char* mName;
		qResourceData* mDefaultResourceData;
		qTreeRB<qResourceData, 0> mResourceDatas;
		qList<qResourceHandle> mNullHandles;
		qList<qResourceHandle> mInternalUnresolvedHandles[4];
		qList<qResourceHandle>* mUnresolvedHandleLists;
		u32 mNumUnresolvedHandleLists;
		u32 mNumResourceData;
		u32 mNumResourceBytes;
		u32 mTransactionNum;
		u32 mLastUpdate;
		f32 mAddTime;
		f32 mRemoveTime;
		f32 mUnresolvedTime;
		f32 mLoadTime;
		f32 mUnloadTime;
		f32 mInitHandleTime;

		qResourceInventory(const char* name, u32 type_uid, u32 chunk_uid, u32 default_name_uid = 0, u32 num_unsolved_hash_lists = 0);

		virtual void InitHandle(qResourceHandle* handle, u32 name_uid);
		virtual void InitHandle(qResourceHandle* handle, u32 name_uid, qResourceData* data);

		virtual void OnAttachHandle(qResourceHandle* handle, qResourceData* data) {};
		virtual void OnDetachHandle(qResourceHandle* handle, qResourceData* data) {};
		virtual void OnPreMove(qResourceData* resourceData) {};
		virtual void OnPostMove(qResourceData* resourceData) {};

		virtual void Init();
		virtual void Close();

		virtual bool IsEmpty() { return mNumResourceData == 0; }

		virtual void Add(qResourceData* data);
		virtual void Remove(qResourceData* data);

		virtual qResourceData* Get(u32 name_uid);
		virtual qResourceData* DebugGet(const char* name);

		virtual bool Load(qChunk* chunk);
		virtual bool Unload(qChunk* chunk);

		virtual bool PreMove(qChunk* chunk);
		virtual bool PostMove(qChunk* chunk);

		virtual bool Validate() { return true; }

		virtual void PrintContents();

		qList<qResourceHandle>* GetResourceHandleList(u32 name_uid, qResourceData* data = 0)
		{
			if (!data || data == mDefaultResourceData)
			{
				if (name_uid == -1 || name_uid == 0) {
					return &mNullHandles;
				}

				return &mUnresolvedHandleLists[name_uid % mNumUnresolvedHandleLists];
			}
			
			return &data->mResourceHandles;
		}
	};

	//-------------------------------------------------------------------
	//	Warehouse
	//-------------------------------------------------------------------

	class qResourceWarehouse
	{
	public:
		qTreeRB<qResourceInventory, 0> mInventoryTree;
		qList<qResourceInventory> mInventoryList;
		qResourceInventory* mLastInventory;
		u32 mLastTypeUID;
		int mNumInventories;
		f32 mAddTime;
		f32 mRemoveTime;
		f32 mUnresolvedTime;
		f32 mLoadTime;
		f32 mUnloadTime;

		static qResourceWarehouse* Instance();

		void Init();
		
		qResourceInventory* GetInventory(u32 type_uid);

		THEORY_INLINE void AddInventory(qResourceInventory* inv)
		{
			mInventoryTree.mTree.Add(&inv->mNode);
			mInventoryList.Insert(inv);
			++mNumInventories;
		}
	};

	template <typename T>
	THEORY_INLINE T* qGetResourceInventory(u32 typeUID)
	{
		static T* result;
		if (!result) {
			result = static_cast<T*>(qResourceWarehouse::Instance()->GetInventory(typeUID));
		}
		return result;
	}

#ifdef THEORY_IMPL

	//-------------------------------------------------------------------
	//	Resource Handle
	//-------------------------------------------------------------------

	void qResourceHandle::Close()
	{
		RemoveFromList();

		if (mData)
		{
			auto inventory = qResourceWarehouse::Instance()->GetInventory(mData->mTypeUID);
			inventory->OnDetachHandle(this, mData);

			mData = 0;
		}
	}


	void qResourceHandle::Close(qResourceInventory* inventory)
	{
		RemoveFromList();

		if (mData)
		{
			inventory->OnDetachHandle(this, mData);

			mData = 0;
		}
	}

	void qResourceHandle::Init(u32 type_uid, u32 name_uid)
	{
		Close();

		if (auto inventory = qResourceWarehouse::Instance()->GetInventory(type_uid)) {
			inventory->InitHandle(this, name_uid);
		}
	}

	void qResourceHandle::Init(u32 type_uid, u32 name_uid, qResourceData* resource_data, qResourceInventory* inventory)
	{
		Close();

		if (inventory) {
			inventory->InitHandle(this, name_uid, resource_data);
		}
	}

	void qResourceHandle::Init(u32 type_uid, u32 name_uid, qResourceInventory* inventory)
	{
		Close();

		if (inventory) {
			inventory->InitHandle(this, name_uid);
		}
	}

	bool qResourceHandle::IsDefault()
	{
		if (!mData) {
			return false;
		}

		auto defaultResourceData = qResourceWarehouse::Instance()->GetInventory(mData->mTypeUID)->mDefaultResourceData;
		if (!defaultResourceData || defaultResourceData != mData) {
			return false;
		}

		return true;
	}

	//-------------------------------------------------------------------
	//	Resource Data
	//-------------------------------------------------------------------

	void qResourceData::SetDebugName(const char* name)
	{
		qMemSet(mDebugName, 0, sizeof(mDebugName));

		if (name)
		{
			int len = qStringLength(name);
			if (len >= sizeof(mDebugName))
			{
				/* Split debug name */
				const int split = 20;
				const int endLen = (sizeof(mDebugName) - split - 2);

				qMemCopy(mDebugName, name, static_cast<usize>(split));

				mDebugName[split] = '~';

				qMemCopy(&mDebugName[split + 1], &name[len - endLen], static_cast<usize>(endLen));
			}
			else {
				qMemCopy(mDebugName, name, static_cast<usize>(len));
			}
		}
	}

	//-------------------------------------------------------------------
	//	Inventory
	//-------------------------------------------------------------------

	qPrintChannel gInventoryChannel = { "qResourceInventory", qPrintChannel::OUTPUT_LEVEL_DEBUG };

	qResourceInventory::qResourceInventory(const char* name, u32 type_uid, u32 chunk_uid, u32 default_name_uid, u32 num_unsolved_hash_lists)
	{
		mNode.SetUID(type_uid);

		mName = name;
		mChunkUID = chunk_uid;

		mDefaultResourceData = nullptr;
		mDefaultResourceNameUID = default_name_uid;

		mUnresolvedHandleLists = mInternalUnresolvedHandles;
		mNumUnresolvedHandleLists = 4;

		if (num_unsolved_hash_lists > 4)
		{
			mUnresolvedHandleLists = new (GetMainMemoryPool()->Allocate(sizeof(UFG::qList<qResourceHandle>) * num_unsolved_hash_lists, "qResourceInventory.Hash", 0, true)) UFG::qList<qResourceHandle>();
			mNumUnresolvedHandleLists = num_unsolved_hash_lists;
		}

		mNumResourceData = 0;
		mNumResourceBytes = 0;
		mTransactionNum = 0;
		mLastUpdate = 0;
		mAddTime = 0.f;
		mRemoveTime = 0.f;
		mUnresolvedTime = 0.f;
		mLoadTime = 0.f;
		mUnloadTime = 0.f;
		mInitHandleTime = 0.f;

		qResourceWarehouse::Instance()->AddInventory(this);
	}

	void qResourceInventory::InitHandle(qResourceHandle* handle, u32 name_uid)
	{
		if (name_uid == -1 || name_uid == 0) {
			return;
		}

		InitHandle(handle, name_uid, Get(name_uid));
	}

	void qResourceInventory::InitHandle(qResourceHandle* handle, u32 name_uid, qResourceData* data)
	{
		GetResourceHandleList(name_uid, data)->Insert(handle);

		handle->mData = data;
		handle->mNameUID = name_uid;

		OnAttachHandle(handle, data);
	}

	void qResourceInventory::Init()
	{
		gInventoryChannel.Print(qPrintChannel::OUTPUT_LEVEL_DEBUG, "[%s] Init()\n", mName);
	}

	void qResourceInventory::Close()
	{
		gInventoryChannel.Print(qPrintChannel::OUTPUT_LEVEL_DEBUG, "[%s] Close()\n", mName);

		if (mResourceDatas.IsEmpty()) {
			return;
		}

		for (auto data : mResourceDatas)
		{
			gInventoryChannel.Print(qPrintChannel::OUTPUT_LEVEL_DEBUG, "[%s] WARNING: Remaining resource name = '%s'\n", mName, data->mDebugName);
			Remove(data);
		}
	}


	void qResourceInventory::Add(qResourceData* data)
	{
		// TODO: Implement "Hot Swap Mode"?
		/*
		if (smHotSwapMode)
		{
			// ....
		}
		*/

		auto dataUID = data->mNode.mUID;
		mResourceDatas.Add(data);

		// TODO: Implement fix for unresolved handles...

		const char* str = "";
		if (mDefaultResourceData == data) {
			str = " - *DEFAULT* ";
		}
		
		++mNumResourceData;
		++mTransactionNum;
		gInventoryChannel.Print(qPrintChannel::OUTPUT_LEVEL_DEBUG, "[%s] ADD%s - 0x%08x - %s\n", mName, str, dataUID, data->mDebugName);
	}

	void qResourceInventory::Remove(qResourceData* data)
	{
		auto dataUID = data->mNode.mUID;
		mResourceDatas.Remove(data);

		bool isDefaultResource = (mDefaultResourceData == data);

		if (isDefaultResource) {
			mDefaultResourceData = 0;
		}

		auto defaultResourceData = mDefaultResourceData;
		auto resourceHandleList = GetResourceHandleList(dataUID);

		for (auto& handle : data->mResourceHandles)
		{
			handle.RemoveFromList();

			OnDetachHandle(&handle, data);

			handle.mData = defaultResourceData;

			resourceHandleList->Insert(&handle);
		}

		// TODO: Implement this...
		/*
		if (v8)
		{
			v14 = this->mDefaultResourceData;
			if (this->mNumUnresolvedHandleLists)
			{
				do
				{
					v15 = &this->mUnresolvedHandleLists[v6];
					for (i = (UFG::qList<UFG::qResourceHandle, UFG::qResourceHandle, 1, 0> *)v15->mNode.mNext;
						i != v15;
						i = (UFG::qList<UFG::qResourceHandle, UFG::qResourceHandle, 1, 0> *)i->mNode.mNext)
					{
						i[1].mNode.mPrev = (UFG::qNode<UFG::qResourceHandle, UFG::qResourceHandle> *)v14;
					}
					++v6;
				} while (v6 < this->mNumUnresolvedHandleLists);
			}
		}	
		*/
		
		--mNumResourceData;
		++mTransactionNum;

		gInventoryChannel.Print(qPrintChannel::OUTPUT_LEVEL_DEBUG, "[%s] REMOVE - 0x%08x - %s\n", mName, dataUID, data->mDebugName);
	}


	qResourceData* qResourceInventory::Get(u32 name_uid)
	{
		return mResourceDatas.Get(name_uid);
	}

	qResourceData* qResourceInventory::DebugGet(const char* name)
	{
		for (auto data : mResourceDatas)
		{
			if (qStringCompareInsensitive(name, data->mDebugName)) {
				continue;
			}

			return data;
		}

		return nullptr;
	}


	bool qResourceInventory::Load(qChunk* chunk)
	{
		if (!mChunkUID || chunk->mUID != mChunkUID) {
			return false;
		}

		Add(chunk->GetData());
		mNumResourceBytes += chunk->mDataSize;

		return true;
	}

	bool qResourceInventory::Unload(qChunk* chunk)
	{
		if (!mChunkUID || chunk->mUID != mChunkUID) {
			return false;
		}

		Remove(chunk->GetData());
		mNumResourceBytes -= chunk->mDataSize;

		return true;
	}

	bool qResourceInventory::PreMove(qChunk* chunk)
	{
		if (!Load(chunk)) {
			return false;
		}

		OnPostMove(chunk->GetData());
		return true;
	}

	bool qResourceInventory::PostMove(qChunk* chunk)
	{
		if (!mChunkUID || !chunk || chunk->mUID != mChunkUID) {
			return false;
		}

		OnPreMove(chunk->GetData());
		Unload(chunk);
		return true;
	}

	void qResourceInventory::PrintContents()
	{
		qPrintf("-------------------------------------------------\n\n");
		qPrintf("Contents of Inventory '%.64s' %d items\n", mName, mNumResourceData);

		for (auto data : mResourceDatas)
		{
			qPrintf("  - Resource TypeUID:0x%08x DataUID:0x%08x Memory:0x%08x Name:'%.36s'\n",
				data->mTypeUID,
				data->mNode.mUID,
				data,
				data->mDebugName);

			// TextureScriberPC64:
			//qPrintf("  - Resource DataUID:0x%08x Memory:0x%08x-0x%08x Size:0x%08x (%9u) Name:'%.36s'\n");
		}

		qPrintf("-------------------------------------------------\n\n");
	}

	//-------------------------------------------------------------------
	//	Warehouse
	//-------------------------------------------------------------------

	qResourceWarehouse* qResourceWarehouse::Instance()
	{
		static qResourceWarehouse sResourceWarehouse;
		return &sResourceWarehouse;
	}

	void qResourceWarehouse::Init()
	{
		for (auto inventory : mInventoryTree)
		{
			inventory->Init();
		}
	}

	qResourceInventory* qResourceWarehouse::GetInventory(u32 type_uid)
	{
		if (mLastInventory && mLastTypeUID == type_uid) {
			return mLastInventory;
		}

		auto inventory = mInventoryTree.Get(type_uid);

		mLastTypeUID = type_uid;
		mLastInventory = inventory;

		return inventory;
	}

#endif
}