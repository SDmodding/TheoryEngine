#pragma once

namespace UFG
{
	class qPropertyList;
	class qPropertySet;
	class qPropertySetResource;

	//----------------------------
	//	Memory Pool
	//----------------------------

	inline qMemoryPool gPropertySetMemoryPool;

	THEORY_INLINE void* qPropertySet_Allocate(u32 byte_count, const char* dbg_tag, u32 flags = 0) {
		return gPropertySetMemoryPool.Allocate(byte_count, dbg_tag, flags, 1);
	}

	THEORY_INLINE void qPropertySet_Free(void* buffer) { gPropertySetMemoryPool.Free(buffer); }

	//----------------------------

	class qProperty
	{
	public:
		union
		{
			u32 mTypeUIDOffsetChanged;

			struct
			{
				u32 mOffset : 24;
				u32 mTypeUID : 8;
			};
		};

		u32 mNameUID;

		u32 GetDataOffset() { return mOffset; }
		void SetDataOffset(u32 dataOffset) { mOffset = dataOffset; }

		u32 GetTypeUID() { return mTypeUID; }
		void SetTypeUID(u32 typeUID) { mTypeUID = typeUID; }
	};

	class qPropertyType
	{
	public:
		const char* mName;
		const char* mXMLTag;
		u32 mNameUID;
		u32 Alignment;
		u32 mByteSize;
		u32 mElementTypeUID;
		u32 mNumElements;

		static qPropertyType* Get(u32 type_name_uid);
		bool IsPtrType(u32 type_uid) { return type_uid == UID_propertyset || type_uid == UID_list || type_uid == UID_string; }
	};

	class qPropertyCollection
	{
	public:
		enum Flags
		{
			FLAG_MEMIMAGE = (1 << 0),
			FLAG_IS_SET = (1 << 1),
			FLAG_IS_LIST = (1 << 2),
			FLAG_DELETED = (1 << 3),
			FLAG_OWNER_IS_SET = (1 << 4),
			FLAG_OWNER_IS_LIST = (1 << 5)
		};

		u32 mFlags;
		u32 mPad0;
		qOffset64<qPropertyCollection*> mOwner;

		qPropertyCollection() : mPad0(0), mOwner(0) {}
		qPropertyCollection(MemImageLoadFlag) {}

		/* Functions */

		bool GetFlags(int flags) const { return mFlags & flags; }
		void SetFlags(int flags) { mFlags |= flags; }

		const qPropertySet* GetOwningSet() const { return GetFlags(FLAG_OWNER_IS_SET) ? reinterpret_cast<qPropertySet*>(mOwner.Get()) : 0; }
		const qPropertyList* GetOwningList() const { return GetFlags(FLAG_OWNER_IS_LIST) ? reinterpret_cast<qPropertyList*>(mOwner.Get()) : 0; }

		bool IsMemImaged() const { return mFlags & FLAG_MEMIMAGE; }
		bool IsPropertySet() const { return (mFlags >> 1) & FLAG_IS_SET; }
		bool IsPropertyList() const { return (mFlags >> 2) & FLAG_IS_LIST; }

		void SetOwner(qPropertySet* owningSet)
		{
			mOwner.Set(owningSet);
			mFlags &= ~FLAG_OWNER_IS_LIST;
			mFlags |= FLAG_OWNER_IS_SET;
		}

		void SetOwner(qPropertyList* owningList)
		{
			mOwner.Set(owningList);
			mFlags &= ~FLAG_OWNER_IS_SET;
			mFlags |= FLAG_OWNER_IS_LIST;
		}
	};

	class qPropertySetHandle : public qTypedResourceHandle<RTypeUID_qPropertySetResource, qPropertySetResource>
	{
	public:
		qPropertySet* Get();
	};

	class qPropertySet : public qPropertyCollection, public qNode<qPropertySet>
	{
	public:
		enum Type
		{
			Unknown,
			Archetype,
			SceneObject,
			LayerProperties,
			Asset
		};

		enum Flags
		{
			FLAG_RESOURCE_SET = (1 << 0),
			FLAG_HAS_SCHEMA = (1 << 1),
			FLAG_IS_SCHEMA = (1 << 2),
			FLAG_INHERIT_SCHEMA = (1 << 3),
			FLAG_COMPONENT_SCHEMA = (1 << 4),
			FLAG_CS_SKIP_PARENT_CHECK = (1 << 5),
			FLAG_REQUIRES_RECURSIVE_SETUP = (1 << 6),
			FLAG_TYPE_START = (1 << 16),
		};

		qOffset64<qPropertySetHandle*> mParents;
		qOffset64<u8*> mValues;
		qOffset64<u32*> mDefaultBits;
		qOffset64<qProperty*> mProperties;
		qSymbol mName;
		u16 mRefCount;
		u16 mNumParents;
		u32 mParentMask;
		qSymbol mSchemaName;
		u32 mPropertyMask;
		u16 mNumDataBytes;
		u16 mNumProperties;

		/* Constructors, Destructor */

		qPropertySet() {}

		qPropertySet(const qSymbol& name) : mName(name),
			mParents(0),
			mProperties(0),
			mValues(0),
			mDefaultBits(0),
			mNumDataBytes(0),
			mRefCount(0),
			mNumProperties(0),
			mNumParents(0)
		{
			mFlags = 0;
			mPropertyMask = -1;
			mParentMask = -1;
			mSchemaName.set_null();
		}

		/* Operators */

		static void* operator new(usize, void* ptr) { return ptr; }
		static void* operator new(usize size, const char* name) { return qPropertySet_Allocate(static_cast<u32>(size), name); }
		static void* operator new[](usize size, const char* name) { return qPropertySet_Allocate(static_cast<u32>(size), name); }

		static void operator delete(void*, void*) {}
		static void operator delete(void*, const char*) {}
		static void operator delete(void* ptr, usize) { qPropertySet_Free(ptr); }
		static void operator delete[](void* ptr, usize) { qPropertySet_Free(ptr); }

		/* Static Functions */

		static qPropertySet* CreateResourceSet(const qSymbol& name, const char* dbg_tag = "CreatePropertySet");

		/* Functions */

		u32 NumParents() { return mNumParents; }
		u32 NumProperties() { return mNumProperties; }

		const void* GetMemImagePtr() const { return mValues.Get(); }

		bool IsResourceSet() const { return GetFlags() & FLAG_RESOURCE_SET; }

		const qPropertySetResource* GetResource() const
		{
			if (!IsResourceSet()) {
				return 0;
			}

			//return reinterpret_cast<qPropertySetResource*>(reinterpret_cast<uptr>(this) - offsetof(qPropertySetResource, mData));
			return reinterpret_cast<qPropertySetResource*>(reinterpret_cast<uptr>(this) - 0x68);
		}

		u16 GetFlags() const { return reinterpret_cast<const u16*>(&mFlags)[1]; }

		Type GetType() const { return static_cast<Type>(mFlags >> 28); }
		void SetType(Type type) { mFlags &= 0xFFFFFFFu; mFlags |= type << 28; }

		const qSymbol& GetName() const { return mName; }
		const char* GetNameStringDBG() const { return mName.as_cstr_dbg(); }
		void SetName(const qSymbol& name) { mName = name; }

		const qSymbol& GetSchemaName() const { return mSchemaName; }

		qPropertySet* GetParentFromIdx(u32 i) { return mParents[i]->Get(); }
	
		void AddRef() { ++mRefCount; }
		void ReleaseRef();

		void Destroy();

		qProperty* FindPropertyLocal(u32 name_uid);

		/* Getters */

		int GetPropertyIdxLocal(u32 type_uid, u32 name_uid);

		int GetPropertyIndexFromName(const qSymbol& name) const
		{
			for (int i = 0; mNumProperties > i; ++i)
			{
				if (mProperties[i]->mNameUID == name) {
					return i;
				}
			}

			return -1;
		}

		const qSymbol& GetPropertyNameFromIndex(u32 index) const
		{
			if (index >= mNumProperties) {
				return gNullQSymbol;
			}

			return mProperties[index]->mNameUID;
		}

		ePropertyType GetPropertyTypeFromIndex(u32 index) const
		{
			if (index >= mNumProperties) {
				return UID_Invalid;
			}

			return static_cast<ePropertyType>(mProperties[index]->GetTypeUID());
		}

		qPropertySet* GetParentFromName(const qSymbol& propertySetName, qPropertyDepth depth = DEPTH_RECURSE);

		void* GetValuePtr(u32 type_uid, u32 name_uid, qPropertyDepth depth = DEPTH_RECURSE, qPropertySet** owningSet = 0);
		void* GetValuePtr2(u32 name_uid, qPropertyDepth depth = DEPTH_RECURSE, qPropertySet** owningSet = 0, u32* type_uid = 0);

		// TODO: Implement setters & getters with template typename 'Get<T>(const qSymbol& name, qPropertyDepth depth);'
		// - The 'Get<T>' function just calls GetValuePtr with the appropriate type_uid based on the type name.
	};

	class qPropertySetResource : public qResourceData
	{
	public:
		enum Flags
		{
			FLAG_MEMIMAGE = 1,
			FLAG_IN_INVENTORY = 2
		};

		u32 mFlags;
		u32 mSourceCRC;
		qOffset64<const char*> mNameString;
		qPropertySet mData;

		/* Constructors, Destructor */

		qPropertySetResource() {}

		qPropertySetResource(const char* set_name) : qResourceData(RTypeUID_qPropertySetResource, qStringHash32(set_name), set_name),
			mData(qSymbol::create_from_string(set_name)),
			mFlags(0),
			mSourceCRC(-1),
			mNameString(0)
		{
			mData.SetFlags((qPropertySet::FLAG_RESOURCE_SET << 16));
			mData.SetType(qPropertySet::Archetype);
			SetName(set_name);
		}

		~qPropertySetResource()
		{
			if (!IsMemImaged()) {
				SetName(0);
			}
		}

		/* Operators */

		static void* operator new(usize, void* ptr) { return ptr; }
		static void* operator new(usize size, const char* name) { return qPropertySet_Allocate(static_cast<u32>(size), name); }
		static void* operator new[](usize size, const char* name) { return qPropertySet_Allocate(static_cast<u32>(size), name); }

		static void operator delete(void*, void*) {}
		static void operator delete(void*, const char*) {}
		static void operator delete(void* ptr, usize) { qPropertySet_Free(ptr); }
		static void operator delete[](void* ptr, usize) { qPropertySet_Free(ptr); }
			
		/* Functions */

		void ClearFlags(int flags) { mFlags &= ~flags; }
		int GetFlags(int flags) const { return mFlags & flags; }
		void SetFlags(int flags) { mFlags |= flags; }

		void SetSourceCRC(u32 sourceCRC) { mSourceCRC = sourceCRC; }

		const char* GetNameString() const { return mNameString.Get(); }
		void SetName(const char* set_name);

		qPropertySet* GetPropertySet() { return &mData; }
		bool IsMemImaged() const { return mFlags & FLAG_MEMIMAGE; }

		void Rename(const char* nameString);

		//qPropertySet* LoadSetFromXMLNode(XMLDocument* xml_doc, XMLNode* parent_node, qPropertySet* existing_set, bool clearExistingSet);
	};

	//----------------------------
	//	Resource Inventory
	//----------------------------

	class qPropertySetInventory : public qResourceInventory
	{
	public:
		qArray<u32> mPurgedResources;

		qPropertySetInventory() : qResourceInventory("qPropertySetInventory", RTypeUID_qPropertySetResource, ChunkUID_qPropertySetResource) {}

		void Add(qResourceData* data) override;
		void Remove(qResourceData* data) override;
	} inline gPropertySetInventory;
}