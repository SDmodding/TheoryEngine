#pragma once

namespace Illusion
{
	class SoftbodyData;

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
		UFG::qOffset<void*> mData;
		u32 mElementByteSize;
		u32 mNumElements;
		UFG::qOffset<BufferUser*> mBufferUser;
		u32 mLastUsedFrameNum;
		u32 pad0;
		UFG::qMemoryPool* mMemoryPool;
		u32 pad1;
		u32 pad2;

		Buffer(u32 name_uid, const char* name, u32 byte_size) : UFG::qResourceData(RTypeUID_Buffer, name_uid, name),
			mDataByteSize(byte_size), mLastUsedFrameNum(0), mMemoryPool(0), pad0(0), pad1(0), pad2(0) {}

		THEORY_INLINE BufferPlat* GetPlat() { return reinterpret_cast<BufferPlat*>(&this[1]); }
	};

	class BufferHandle : public UFG::qTypedResourceHandle<RTypeUID_Buffer, Buffer>
	{
	public:
	};

	//-------------------------------------------------------------------

	class Locators : public UFG::qResourceData
	{
	public:
		u32 mNumLocators;
		UFG::qOffset<char*> mNameTable;
		UFG::qOffset<UFG::qMatrix44*> mTransformTable;
	};

	class LocatorsHandle : public UFG::qTypedResourceHandle<RTypeUID_Locators, Locators>
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
		UFG::qOffset<u32*> mTargetUIDTable;
		UFG::qOffset<char*> mTargetNameTable;
	};

	class MorphTargetsHandle : public UFG::qTypedResourceHandle<RTypeUID_MorphTargets, MorphTargets>
	{
	public:
	};

	//-------------------------------------------------------------------
	// Mesh
	//-------------------------------------------------------------------
	
	class MeshPlat
	{
	public:
		int mPad0;
	};

	class IMeshPlat
	{
	public:
	};

	class Mesh : public IMeshPlat
	{
	public:
		MaterialHandle mMaterialHandle;
		VertexDeclHandle mVertexDeclHandle;
		BufferHandle mIndexBufferHandle;
		BufferHandle mVertexBufferHandles[4];
		int mPrimType;
		int mIndexStart;
		u32 mNumPrims;
		u32 pad;
		const char* mDescription;

		Mesh() {}
	};

	//-------------------------------------------------------------------
	// Model
	//-------------------------------------------------------------------

	class ModelUser
	{
	public:
		UFG::qOffset<SoftbodyData*> mSoftbodyData;
		u32 mHasSoftbodyInfo;
	};

	class ModelUserPlat
	{
	public:
		int mExampleOfModelUserPlatData1;
		float mExampleOfModelUserPlatData2;
	};

	class ModelPlat
	{
	public:
		UFG::qOffset<ModelUserPlat*> mModelUserPlat;
	};

	class IModelPlat
	{
	public:
	};

	class Model : public IModelPlat, public UFG::qResourceData
	{
	public:
		float mAABBMin[3];
		u32 mNumPrims;
		float mAABBMax[3];
		u32 pad0;
		MaterialTableHandle mMaterialTableHandle;
		BonePaletteHandle mBonePaletteHandle;
		UFG::qOffset<UFG::qOffset<Mesh*>*> mMeshOffsetTable;
		u32 mNumMeshes;
		UFG::qOffset<ModelUser*> mModelUser;
		u32 pad1;
		u32 mLastUsedFrameNum;
		u32 pad2;
		UFG::qMemoryPool* mMemoryPool;
		MorphTargetsHandle mMorphTargetsHandle;
		LocatorsHandle mLocatorsHandle;

		Model(u32 name_uid, const char* name) : UFG::qResourceData(RTypeUID_Model, name_uid, name),
			mMemoryPool(0), pad0(0), pad1(0), pad2(0) {}

		THEORY_INLINE Mesh* GetMesh(u32 index) { return mMeshOffsetTable[index]->Get(); }
	};

	class ModelHandle : public UFG::qTypedResourceHandle<RTypeUID_Model, Model>
	{
	public:
	};
}