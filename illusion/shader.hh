#pragma once

namespace Illusion
{
	class IShaderPlat {};
	class ShaderUser;
	class ShaderParam;

	class Shader : public IShaderPlat, public UFG::qResourceData
	{
	public:
		enum StageType
		{
			STAGE_VERTEX,
			STAGE_PIXEL,
			STAGE_COMPUTE,
			STAGE_GEOMETRY,
			STAGE_HULL,
			STAGE_DOMAIN,
			MAX_STAGES,

			STAGE_INVALID = 99
		};

		UFG::BitFlags128 mShaderParamInputMask;
		UFG::qOffset64<ShaderUser*> mShaderUser;
		int mSortID;
		ShaderParam* mLocalShaderParams;
		u32 mNumLocalShaderParams;
		void* mShaderParam[128];
	};

	class ShaderHandle : public UFG::qTypedResourceHandle<RTypeUID_Shader, Shader> {};

	class ShaderBinary : public UFG::qResourceData
	{
	public:
		u32 mDataByteSize;
		u32 mShaderStageType;
		UFG::qOffset64<void*> mData;
		char mShaderName[52];
	};

	class ShaderBinaryHandle : public UFG::qTypedResourceHandle<RTypeUID_ShaderBinary, ShaderBinary> {};
}