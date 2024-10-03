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
		enum BufferFlags
		{
			FLAG_CPU_READABLE		= (1 << 0),
			FLAG_CPU_WRITABLE		= (1 << 1),
			FLAG_FRAME_GENERATED	= (1 << 2),
			FLAG_SHADER_RESOURCE	= (1 << 3),
			FLAG_SHADER_UAV			= (1 << 4),
			FLAG_SHADER_STRUCTURED	= (1 << 5),
			FLAG_SHADER_ALLOW_RAW	= (1 << 6)
		};

		enum BufferType
		{
			TYPE_VERTEX,
			TYPE_INDEX,
			TYPE_INSTANCE,
			NUM_TYPES
		};

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