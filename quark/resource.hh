#pragma once

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

		inline qResourceData* GetData()
		{
			return reinterpret_cast<qResourceData*>(reinterpret_cast<uptr>(this) + mDataOffset + sizeof(qChunk));
		}
	};

	//-------------------------------------------------------------------
	// Resource Handle
	//-------------------------------------------------------------------

	class qResourceHandle : public qNode<qResourceHandle, qResourceHandle>
	{
	public:
		qResourceData* mData;
		u32 mNameUID;
		u32 mTailPad;

		inline qResourceHandle() : mData(0), mNameUID(0), mTailPad(0) {}

		inline ~qResourceHandle()
		{
			Close();
		}

		void Close();

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
	};

	//-------------------------------------------------------------------
	// Resource Data
	//-------------------------------------------------------------------

	class qResourceData : public qNodeRB<qResourceData>
	{
	public:
		qList<qResourceHandle, qResourceHandle> mResourceHandles;
		u32 mTypeUID;
		char mDebugName[36];

		inline qResourceData() {}
		qResourceData(u32 type_uid, u32 name_uid, const char* name);

		void SetDebugName(const char* name);
	};

	//-------------------------------------------------------------------
	// Inventory
	//-------------------------------------------------------------------

	class qResourceInventory : public qNodeRB<qResourceInventory>, public qNode<qResourceInventory, qResourceInventory>
	{
	public:
		u32 mDefaultResourceNameUID;
		u32 mChunkUID;
		const char* mName;
		qResourceData* mDefaultResourceData;
		qTreeRB<qResourceData> mResourceDatas;
		qList<qResourceHandle, qResourceHandle> mNullHandles;
		qList<qResourceHandle, qResourceHandle> mInternalUnresolvedHandles[4];
		qList<qResourceHandle, qResourceHandle>* mUnresolvedHandleLists;
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

		// TODO: Need finish functions & virtual functions.

		qResourceInventory(const char* name, u32 type_uid, u32 chunk_uid, u32 default_name_uid, u32 num_unsolved_hash_lists);

		virtual void InitHandle(qResourceHandle* handle, u32 name_uid);
		virtual void InitHandle(qResourceHandle* handle, u32 name_uid, qResourceData* data);
	};

	//-------------------------------------------------------------------
	// Warehouse
	//-------------------------------------------------------------------

	class qResourceWarehouse
	{
	public:
		qTreeRB<qResourceInventory> mInventoryTree;
		qList<qResourceInventory, qResourceInventory> mInventoryList;
		qResourceInventory* mLastInventory;
		u32 mLastTypeUID;
		int mNumInventories;
		f32 mAddTime;
		f32 mRemoveTime;
		f32 mUnresolvedTime;
		f32 mLoadTime;
		f32 mUnloadTime;

		static qResourceWarehouse* Instance();

		THEORY_INLINE void AddInventory(qResourceInventory* inv)
		{
			mInventoryTree.mTree.Add(&inv->mNode);
			mInventoryList.Insert(inv);
		}
	};


#ifdef THEORY_IMPL

	//-------------------------------------------------------------------
	// Resource Handle
	//-------------------------------------------------------------------

	void qResourceHandle::Close()
	{
		RemoveNode();

		if (mData)
		{
			// TODO: GetInventory from Warehouse and call OnDetachHandle.
			mData = nullptr;
		}
	}


	void qResourceHandle::Init(u32 type_uid, u32 name_uid)
	{
		Close();
		// TODO: GetInventory from Warehouse and call InitHandle.
	}

	void qResourceHandle::Init(u32 type_uid, u32 name_uid, qResourceData* resource_data, qResourceInventory* inventory)
	{
		Close();
		// TODO: GetInventory from Warehouse and call InitHandle.
	}

	void qResourceHandle::Init(u32 type_uid, u32 name_uid, qResourceInventory* inventory)
	{
		Close();
		// TODO: GetInventory from Warehouse and call InitHandle.
	}

	//-------------------------------------------------------------------
	// Resource Data
	//-------------------------------------------------------------------

	qResourceData::qResourceData(u32 type_uid, u32 name_uid, const char* name)
	{
		mNode.mUID = name_uid;
		mTypeUID = type_uid;
		SetDebugName(name);
	}

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
	// Inventory
	//-------------------------------------------------------------------

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
		// TODO: Implement this.
	}

	void qResourceInventory::InitHandle(qResourceHandle* handle, u32 name_uid, qResourceData* data)
	{
		// TODO: Implement this.
	}

	//-------------------------------------------------------------------
	// Warehouse
	//-------------------------------------------------------------------

	qResourceWarehouse* qResourceWarehouse::Instance()
	{
		static qResourceWarehouse sResourceWarehouse;
		return &sResourceWarehouse;
	}

#endif
}