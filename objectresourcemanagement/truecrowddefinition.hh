#pragma once

namespace UFG
{
	class TrueCrowdDefinition
	{
	public:
		struct Component
		{
			u32 mNameUID;
			char mName[32];
		};

		struct Entity
		{
			struct EntityComponent
			{
				qSymbol mName;
				u32 mResourceIndex;
				u32 mBoneUID[16];
				u32 mNumBoneUIDs;
				u32 mbRequired;
			};

			u32 mNameUID;
			u32 mComponentCount;
			u32 mRequiredComponentCount;
			EntityComponent mComponents[16];
		};

		u32 mComponentCount;
		Component mComponents[25];
		u32 mEntityCount;
		Entity mEntities[25];
		u32 mNumTags;
		qOffset64<qSymbol*> mTagList;
	};
}