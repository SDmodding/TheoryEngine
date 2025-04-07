#pragma once

namespace UFG
{
	class qPropertyList : public qPropertyCollection
	{
	public:
		enum Flags
		{
			FLAG_DELETED = (1 << 16)
		};

		qOffset64<u8*> mValues;
		u32 mTypeUID;
		u32 mElementSize;
		qOffset64<u32*> mWeights;
		u32 mNumElements;
		u32 mTotalWeight;

		/* Constructors, Destructor */

		qPropertyList()
		{
			if (mTypeUID == UID_propertyset)
			{
				// TODO: Loop NumElements and call this->Get<qPropertySet> and call constructor.
			}
			else if (mTypeUID == UID_list)
			{
				// TODO: Loop NumElements and call this->Get<qPropertyList> and call constructor.
			}
		}

		qPropertyList(const char* name) : mValues(0),
			mTypeUID(UID_Invalid),
			mElementSize(0),
			mWeights(0),
			mNumElements(0),
			mTotalWeight(0)
		{
			SetFlags(FLAG_IS_LIST);
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

		static qPropertyList* Create(const char* dbg_tag = "qPropertyList") { return new (dbg_tag)qPropertyList(dbg_tag); }

		/* Functions */

		u32 GetRandomIndex();

		void ReleaseRef() { delete this; }

		/* Helpers */

		THEORY_INLINE u32 GetTypeSize(u32 type_uid)
		{
			auto propertyType = qPropertyType::Get(type_uid);
			return qAlignUp(propertyType->mByteSize, propertyType->Alignment);
		}

		/* Getters */

		void* GetValuePtr(u32 type_uid, u32 index)
		{
			if (type_uid != mTypeUID || index >= mNumElements) {
				return 0;
			}

			return mValues.Get(GetTypeSize(type_uid) * index);
		}
	};

#ifdef THEORY_IMPL

	u32 qPropertyList::GetRandomIndex()
	{
		const int numElements = mNumElements;
		auto weights = mWeights.Get();
		if (!weights) {
			return qRandom(numElements);
		}

		const int numWeights = mTotalWeight;
		if (!numElements) {
			return 0;
		}

		u32 randomWeight = qRandom(numWeights);
		int rand = 0;
		while (1)
		{
			randomWeight -= *weights++;
			if (0 > randomWeight) {
				break;
			}

			++rand;
			if (rand >= numElements) {
				return 0;
			}
		}

		return rand;
	}

#endif
}