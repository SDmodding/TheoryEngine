#pragma once

struct ClassTypeDescriptor
{
	u32 mUID;
	FNInPlaceNew mfnInPlaceNew;
};

ClassTypeDescriptor* GetTypeDescriptors(u32& count);

class TypeTableEntry
{
public:
	u32 mUID;
	FNInPlaceNew mfnInPlaceNew;
};

class TypeTable
{
public:
	u32 mFirstFixupOffset;
	u32 mNumEntries;
	UFG::qOffset64<TypeTableEntry*> mEntries;
};

void Fixup(TypeTable* typeTable, void* resource);

class ActionTreeResource : public UFG::qResourceData
{
public:
	UFG::qOffset64<ActionNodeRoot*> mRootNode;
	UFG::qOffset64<TypeTable*> mTypeTable;
};

class ActionTreeResourceInventory : public UFG::qResourceInventory
{
public:
	ActionTreeResource* mpLastLoadedResource;

	ActionTreeResourceInventory() : qResourceInventory("ActionTreeResourceInventory", RTypeUID_ActionTreeResource, ChunkUID_ActionTreeResource) {}

	void Add(UFG::qResourceData* data) override
	{
		auto actionTreeResource = new (data) ActionTreeResource;	

		Fixup(actionTreeResource->mTypeTable.Get(), data);

		qResourceInventory::Add(data);

		mpLastLoadedResource = actionTreeResource;
	}

	void Remove(UFG::qResourceData* data) override
	{
		auto actionTreeResource = static_cast<ActionTreeResource*>(data);

		qResourceInventory::Remove(data);

		if (mpLastLoadedResource == actionTreeResource) {
			mpLastLoadedResource = 0;
		}
	}
} extern gActionTreeResourceInventory;