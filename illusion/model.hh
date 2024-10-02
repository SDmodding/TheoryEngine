#pragma once

namespace Illusion
{
	class BufferUser
	{
	public:
		int mExampleOfBufferUserData1;
		float mExampleOfBufferUserData2;
	};

	class Buffer : public IBufferPlat, public UFG::qResourceData
	{
	public:
		s8 mBufferType;
		s8 mRunTimeCreated;
		u16 mFlags;
		u32 mDataByteSize;
		UFG::qOffset64<void*> mData;
		u32 mElementByteSize;
		u32 mNumElements;
		UFG::qOffset64<BufferUser*> mBufferUser;
		u32 mLastUsedFrameNum;
		u32 pad0;
		UFG::qMemoryPool* mMemoryPool;
		u32 pad1;
		u32 pad2;
	};
}