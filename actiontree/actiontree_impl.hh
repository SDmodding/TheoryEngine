#pragma once
#define DEFINE_ClassTypeDescriptor(T) { T::sClassNameUID, InPlaceInit<T>::InPlaceNew }

ClassTypeDescriptor* GetTypeDescriptors(u32& count)
{
	static ClassTypeDescriptor gTypeDescriptors[] = {
		DEFINE_ClassTypeDescriptor(ActionNodeReference),
		DEFINE_ClassTypeDescriptor(ActionNodeFileReference),
		DEFINE_ClassTypeDescriptor(ActionNodePlayable),
		DEFINE_ClassTypeDescriptor(ActionNodeBank),
		DEFINE_ClassTypeDescriptor(ActionNodeRoot),

		DEFINE_ClassTypeDescriptor(TrackLibraryNode),

		DEFINE_ClassTypeDescriptor(StimulusBank),

		DEFINE_ClassTypeDescriptor(NISNode),
		DEFINE_ClassTypeDescriptor(NISActorNode),
	};

	count = (sizeof(gTypeDescriptors) / sizeof(gTypeDescriptors[0]));
	return gTypeDescriptors;
}

void Fixup(TypeTable* typeTable, void* resource)
{
	u32 numTypes;
	auto types = GetTypeDescriptors(numTypes);

	auto entries = typeTable->mEntries.Get();
	for (u32 i = 0; typeTable->mNumEntries > i; ++i)
	{
		struct DbgTypeMissing
		{
			u32 mClassNameUID;
			DbgTypeMissing() 
			{
				//UFG::qPrintf("[ DBG ] Fixup missing class nameuid: %X\n", mClassNameUID);
			}
		};

		auto entry = &entries[i];
		entry->mfnInPlaceNew = InPlaceInit<DbgTypeMissing>::InPlaceNew;

		for (u32 j = 0; numTypes > j; ++j)
		{
			if (types[j].mUID == entry->mUID)
			{
				entry->mfnInPlaceNew = types[j].mfnInPlaceNew;
				break;
			}
		}
	}

	int offset = typeTable->mFirstFixupOffset * 4;
	while (offset)
	{
		union FixupData
		{
			union
			{
				struct
				{
					u32 offset : 23;
					u32 entryID : 9;
				};
			};
			void* ptr;
		};

		auto data = reinterpret_cast<FixupData*>(reinterpret_cast<uptr>(resource) + offset);
		offset = (data->offset) * 4;

		auto entry = &entries[data->entryID];
		*reinterpret_cast<u32*>(&data->ptr) = entry->mUID;

		entry->mfnInPlaceNew(&data->ptr);
	}
}

ActionTreeResourceInventory gActionTreeResourceInventory;