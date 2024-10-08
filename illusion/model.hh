#pragma once

namespace Illusion
{
	class BufferUser
	{
	public:
		int mExampleOfBufferUserData1;
		f32 mExampleOfBufferUserData2;
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

		inline Buffer(u32 name_uid, const char* name, u32 byte_size) : UFG::qResourceData(RTYPE_Buffer, name_uid, name),
			mDataByteSize(byte_size), mLastUsedFrameNum(0), mMemoryPool(0), pad0(0), pad1(0), pad2(0) {}
	};

	class BufferHandle : public UFG::qTypedResourceHandle<RTYPE_Buffer, Buffer>
	{
	public:
	};

	//-------------------------------------------------------------------

	class MorphTargets : public UFG::qResourceData
	{
	public:
		BufferHandle mMorphVertexBufferHandle;
		u32 mNumTargets;
		u32 mPad0;
		UFG::qOffset64<u32*> mTargetUIDTable;
		UFG::qOffset64<char*> mTargetNameTable;
	};

	class MorphTargetsHandle : public UFG::qTypedResourceHandle<RTYPE_MorphTargets, Illusion::MorphTargets>
	{
	public:
	};

}