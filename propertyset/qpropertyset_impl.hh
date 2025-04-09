#pragma once

namespace UFG
{
	qPropertyType* qPropertyType::Get(u32 type_name_uid)
	{
		static qPropertyType types[PROPERTY_SET_DATA_TYPE_COUNT] = {
			{ "int8", "Int8", UID_int8, 1, 1, UID_int8, 1 },
			{ "int16", "Int16", UID_int16, 2, 2, UID_int16, 1 },
			{ "int32", "Int32", UID_int32, 4, 4, UID_int32, 1 },
			{ "int64", "Int64", UID_int64, 8, 8, UID_int64, 1 },
			{ "int128", "Int128", UID_int128, 16, 16, UID_int128, 1 },
			{ "uint8", "UInt8", UID_uint8, 1, 1, UID_uint8, 1 },
			{ "uint16", "UInt16", UID_uint16, 2, 2, UID_uint16, 1 },
			{ "uint32", "UInt32", UID_uint32, 4, 4, UID_uint32, 1 },
			{ "uint64", "UInt64", UID_uint64, 8, 8, UID_uint64, 1 },
			{ "bool", "Boolean", UID_bool, 1, 1, 0, 1 },
			{ "float", "Float32", UID_float, 4, 4, UID_float, 1 },
			{ "double", "Float64", UID_double, 8, 8, UID_double, 1 },
			{ "string", "String", UID_string, sizeof(void*), sizeof(void*), 0, 1},
			{ "rangedFloat", "FloatRanged", UID_rangedFloat, 4, 8, UID_float, 2 },
			{ "rangedInt32", "UInt32Ranged", UID_rangedUint32, 4, 8, UID_uint32, 2 },
			{ "rangedInt32", "Int32Ranged", UID_rangedInt32, 4, 8, UID_int32, 2 },
			{ "weightedList", "WeightedListXX", UID_weightedList, 8, 8, UID_uint64, 1 },
			{ "qVector2", "Vector2", UID_qVector2, 4, 8, UID_float, 2 },
			{ "qVector3", "Vector3", UID_qVector3, 4, 12, UID_float, 3 },
			{ "qVector4", "Vector4", UID_qVector4, 4, 16, UID_float, 4 },
			{ "qMatrix44", "TransRotation", UID_qMatrix44, 4, 64, UID_float, 10 },
			{ "qResHandle", "Resource", UID_qResHandle, 4, 40, UID_uint32, 5 },
			{ "qSymbol", "Symbol", UID_qSymbol, 4, 4, UID_uint32, 1 },
			{ "qSymbolUC", "SymbolUC", UID_qSymbolUC, 4, 4, UID_uint32, 1 },
			{ "WwiseID", "WwiseID", UID_qWiseSymbol, 4, 4, UID_uint32, 1 },
			{ "List", "List", UID_list, 8, 8, UID_uint64, 1 },
			{ "PropertySet", "PropSet", UID_propertyset, 8, 8, UID_uint64, 1 },
			{ "Transrotation", "TransRotation", UID_TransRot, 4, 64, UID_float, 16 },
			{ "TransQuat", "TransQuat", UID_qTransQuat, 4, 28, UID_float, 7 },
			{ "List", "WeightedList", UID_list, 8, 8, UID_uint64, 1 }
		};

		return &types[type_name_uid];
	}

	//----------------------------
	//	Property Set Handle
	//----------------------------

	void qPropertySetHandle::Close()
	{
		if (!mNext)
		{
			mData = 0;
			return;
		}

		qResourceHandle::Close(qGetResourceInventory<qPropertySetInventory>(ResourceTypeUID));
	}

	void qPropertySetHandle::CopyFrom(qPropertySetHandle& other)
	{
		if (mNext)
		{
			Init(other.mNameUID);
			return;
		}

		mNext = 0;
		mPrev = 0;
		mData = other.mData;
		mNameUID = other.mNameUID;
	}

	void qPropertySetHandle::Init(u32 resource_uid)
	{
		auto inventory = qGetResourceInventory<qPropertySetInventory>(ResourceTypeUID);
		qResourceHandle::Init(ResourceTypeUID, resource_uid, inventory);
	}

	void qPropertySetHandle::Init() 
	{
		auto inventory = qGetResourceInventory<qPropertySetInventory>(ResourceTypeUID);
		qResourceHandle::Init(ResourceTypeUID, mNameUID, inventory);
	}

	void qPropertySetHandle::InitTempNonResourcePropSet(const qPropertySet* parent) 
	{
		mNext = 0;
		mPrev = 0;
		mData = (qResourceData*)(parent);
		mNameUID = parent->GetName();
	}

	qPropertySet* qPropertySetHandle::Get()
	{
		if (mData && mNext) {
			return static_cast<qPropertySetResource*>(mData)->GetPropertySet();
		}

		return reinterpret_cast<qPropertySet*>(mData);
	}

	//----------------------------
	//	Property Set
	//----------------------------

	qPropertySet::qPropertySet() : qPropertyCollection(MemImageLoadFlag())
	{
		auto parent = mParents.Get();
		const u32 numParents = NumParents();
		for (u32 i = 0; numParents > i; ++i)
		{
			new (parent) (qResourceHandle);
			parent->Init();

			++parent;
		}

		if (GetFlags(FLAG_INHERIT_SCHEMA))
		{
			// TODO: Call `RecursiveSchemaGet` and `SetSchemaName`.
		}

		if (mSchemaName != gNullQSymbol && mSchemaName != GetName())
		{
			// Initialize stuff from `UFG::SchemaDef`?
		}

		if (GetFlags(FLAG_REQUIRES_RECURSIVE_SETUP))
		{
			auto property = mProperties.Get();
			auto values = mValues.Get();

			const u32 numProperties = NumProperties();
			for (u32 i = 0; numProperties > i; ++i)
			{
				if (property->GetTypeUID() == UID_propertyset)
				{
					auto offset = reinterpret_cast<qOffset64<qPropertySet*>*>(&values[property->GetDataOffset()]);
					new (offset->Get()) qPropertySet;
				}
				else if (property->GetTypeUID() == UID_list)
				{
					auto offset = reinterpret_cast<qOffset64<qPropertyList*>*>(&values[property->GetDataOffset()]);
					new (offset->Get()) qPropertyList;
				}

				++property;
			}
		}
	}

	const qPropertySetResource* qPropertySet::GetResource() const
	{
		if (!this || !IsResourceSet()) {
			return 0;
		}

		return reinterpret_cast<qPropertySetResource*>(reinterpret_cast<uptr>(this) - offsetof(qPropertySetResource, mData));
	}

	qPropertySet* qPropertySet::CreateResourceSet(const qSymbol& name, const char* dbg_tag)
	{
		auto propResource = new (dbg_tag) qPropertySetResource(name.as_cstr_dbg());
		return propResource->GetPropertySet();
	}

	void qPropertySet::ReleaseRef()
	{
		if (mRefCount) {
			--mRefCount;
		}

		if (mRefCount || IsMemImaged()) {
			return;
		}

		if (!IsResourceSet())
		{
			delete this;
			return;
		}

		auto propResource = GetResource();
		if (!propResource || PropertySetCache::Contains(propResource)) {
			return;
		}

		delete propResource;
	}

	void qPropertySet::Destroy()
	{
		if (IsMemImaged()) {
			return;
		}

		if (!IsResourceSet())
		{
			delete this;
			return;
		}

		auto propResource = GetResource();
		if (propResource) {
			delete propResource;
		}
	}

	qProperty* qPropertySet::FindPropertyLocal(u32 name_uid)
	{
		auto properties = mProperties.Get();
		const u32 numProperties = NumProperties();

		if ((mPropertyMask & name_uid) == name_uid)
		{
			if (IsMemImaged())
			{
				int index = 0;
				int maxIndex = numProperties - 1;
				while (maxIndex >= index)
				{
					int i = (maxIndex + index) / 2;
					auto property = &properties[i];

					if (name_uid > property->mNameUID)
					{
						index = i + 1;
						continue;
					}

					if (name_uid < property->mNameUID)
					{
						maxIndex = i - 1;
						continue;
					}

					return property;
				}
			}
			else
			{
				for (u32 i = 0; numProperties > i; ++i)
				{
					auto property = &properties[i];
					if (property->mNameUID != name_uid) {
						continue;
					}

					return property;
				}
			}
		}

		return 0;
	}

	int qPropertySet::GetPropertyIdxLocal(u32 type_uid, u32 name_uid)
	{
		auto properties = mProperties.Get();
		const u32 numProperties = NumProperties();

		if ((mPropertyMask & name_uid) == name_uid)
		{
			if (IsMemImaged())
			{
				int index = 0;
				int maxIndex = numProperties - 1;
				while (maxIndex >= index)
				{
					int i = (maxIndex + index) / 2;
					auto property = &properties[i];

					if (name_uid > property->mNameUID)
					{
						index = i + 1;
						continue;
					}

					if (name_uid < property->mNameUID)
					{
						maxIndex = i - 1;
						continue;
					}

					if (property->GetTypeUID() != type_uid) {
						break;
					}

					return i;
				}
			}
			else
			{
				for (u32 i = 0; numProperties > i; ++i)
				{
					auto property = &properties[i];
					if (property->mNameUID != name_uid) {
						continue;
					}

					if (property->GetTypeUID() != type_uid) {
						break;
					}

					return i;
				}
			}
		}
		return -1;
	}

	qPropertySet* qPropertySet::GetParentFromName(const qSymbol& propertySetName, qPropertyDepth depth)
	{
		if ((mParentMask & propertySetName) == propertySetName)
		{
			auto parent = mParents.Get();
			const u32 numParents = NumParents();
			for (u32 i = 0; numParents > i; ++i)
			{
				auto parentSet = parent->Get();
				if (parent->mNameUID == propertySetName) {
					return parentSet;
				}

				if (parentSet)
				{
					if (auto recursiveSet = GetParentFromName(propertySetName, DEPTH_RECURSE)) {
						return recursiveSet;
					}
				}

				++parent;
			}
		}

		return 0;
	}

	void* qPropertySet::GetValuePtr(u32 type_uid, u32 name_uid, qPropertyDepth depth, qPropertySet** owningSet)
	{
		if (auto property = FindPropertyLocal(name_uid))
		{
			if (property->GetTypeUID() == type_uid && owningSet) {
				*owningSet = this;
			}

			return mValues.Get(property->GetDataOffset());
		}

		if (depth == DEPTH_RECURSE)
		{
			auto parent = mParents.Get();
			const u32 numParents = NumParents();
			for (u32 i = 0; numParents > i; ++i)
			{
				if (auto parentSet = parent->Get())
				{
					if (auto valuePtr = GetValuePtr(type_uid, name_uid, DEPTH_RECURSE, owningSet)) {
						return parentSet;
					}
				}

				++parent;
			}
		}

		return 0;
	}

	void* qPropertySet::GetValuePtr2(u32 name_uid, qPropertyDepth depth, qPropertySet** owningSet, u32* type_uid)
	{
		if (auto property = FindPropertyLocal(name_uid))
		{
			if (type_uid) {
				*type_uid = property->GetTypeUID();
			}

			if (owningSet) {
				*owningSet = this;
			}

			return mValues.Get(property->GetDataOffset());
		}

		if (depth == DEPTH_RECURSE)
		{
			auto parent = mParents.Get();
			const u32 numParents = NumParents();
			for (u32 i = 0; numParents > i; ++i)
			{
				if (auto parentSet = parent->Get())
				{
					if (auto valuePtr = GetValuePtr2(name_uid, DEPTH_RECURSE, owningSet, type_uid)) {
						return parentSet;
					}
				}

				++parent;
			}
		}

		return 0;
	}

	//----------------------------
	//	Property Set Resource
	//----------------------------

	void qPropertySetResource::SetName(const char* set_name)
	{
		if (IsMemImaged()) {
			return;
		}

		if (auto name = GetNameString())
		{
			qPropertySet_Free((void*)name);
			mNameString.Set(0);
		}

		if (!set_name) {
			return;
		}

		char* name = static_cast<char*>(qPropertySet_Allocate(qStringLength(set_name) + 1, "qPropertySetResource name"));
		qStringCopy(name, set_name);
		mNameString.Set(name);
	}

	void qPropertySetResource::Rename(const char* nameString)
	{
		const bool inCache = PropertySetCache::Contains(this);
		if (inCache) {
			PropertySetCache::Remove(this);
		}

		SetName(nameString);

		qSymbol name = qSymbol::create_from_string(nameString);

		mNode.SetUID(name);
		SetDebugName(nameString);
		mData.SetName(name);

		if (inCache) {
			PropertySetCache::Add(this);
		}
	}

	//----------------------------
	//	Resource Inventory
	//----------------------------

	void qPropertySetInventory::Add(qResourceData* data)
	{
		auto propertySetResource = static_cast<qPropertySetResource*>(data);
		const bool isMemImaged = propertySetResource->IsMemImaged();

		if (isMemImaged)
		{
			for (u32 purgedUID : mPurgedResources)
			{
				if (purgedUID == data->mNode.mUID) {
					return;
				}
			}

			new(propertySetResource)(qPropertySetResource);
		}

		propertySetResource->SetFlags(qPropertySetResource::FLAG_IN_INVENTORY);
		qResourceInventory::Add(data);

		if (isMemImaged) {
			PostLoadSchemaCreateCheck(propertySetResource->GetPropertySet(), 0, 0);
		}
	}

	void qPropertySetInventory::Remove(qResourceData* data)
	{
		auto propertySetResource = static_cast<qPropertySetResource*>(data);
		if (!propertySetResource->GetFlags(qPropertySetResource::FLAG_IN_INVENTORY)) {
			return;
		}

		qResourceInventory::Remove(data);
		propertySetResource->ClearFlags(qPropertySetResource::FLAG_IN_INVENTORY);

		if (propertySetResource->IsMemImaged()) {
			propertySetResource->~qPropertySetResource();
		}
	}
}