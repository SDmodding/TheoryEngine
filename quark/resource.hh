#pragma once

namespace UFG
{
	class qResourceData;

	class qChunk
	{
	public:
		u32 mUID;
		u32 mChunkSize;
		u32 mDataSize;
		u32 mDataOffset;
	};

	class qResourceHandle : public qNode<qResourceHandle, qResourceHandle>
	{
	public:
		qResourceData* mData;
		u32 mNameUID;
		u32 mTailPad;

		inline qResourceHandle() : mData(0), mNameUID(0), mTailPad(0) {}
	};

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

#ifdef THEORY_IMPL

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

				qMemCopy(mDebugName, name, static_cast<size_t>(split));

				mDebugName[split] = '~';

				qMemCopy(&mDebugName[split + 1], &name[len - endLen], static_cast<size_t>(endLen));
			}
			else {
				qMemCopy(mDebugName, name, static_cast<size_t>(len));
			}
		}
	}

#endif
}