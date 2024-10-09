#pragma once

namespace Illusion
{
	class ParamOverride
	{
	public:
		enum Condition : s32
		{
			OVERRIDE_ALWAYS,
			OVERRIDE_EQUALS,
			OVERRIDE_NOT_EQUALS,
			OVERRIDE_CONSTANT_BLOCK
		};

		int mStateParamIndex;
		Condition mCondition;
		UFG::qGenericResourceHandle mOverrideResourceHandle;
		UFG::qGenericResourceHandle mOriginalResourceHandle;
		void* mConstantBlock;
		u32 mConstantBlockSize;

		ParamOverride() {}
		~ParamOverride() {}

		void SetOverride(u32 param_uid, u32 resource_type_name_uid, u32 override_name_uid, Condition condition, u32 original_name_uid);
		void SetTextureOverride(u32 param_uid, u32 override_name_uid, Condition condition, u32 original_name_uid);
	};

	class MaterialModifierData
	{
	public:
		ParamOverride* mOverridesPtr;
		int mNumOverrides;
	};

	template <int N>
	class MaterialModifierParams : public MaterialModifierData
	{
	public:
		ParamOverride mOverrides[N];
		int mMaxOverrides;
	};

#ifdef THEORY_IMPL

	void ParamOverride::SetOverride(u32 param_uid, u32 resource_type_name_uid, u32 override_name_uid, Condition condition, u32 original_name_uid)
	{
		auto paramIndex = GetStateSystem()->GetParam(param_uid);

		mConstantBlock = nullptr;
		mStateParamIndex = static_cast<int>(paramIndex);
		mConstantBlockSize = 0;
		mCondition = condition;

		mOverrideResourceHandle.SetTypeNameUID(resource_type_name_uid, override_name_uid);
		mOverrideResourceHandle.Init();

		mOriginalResourceHandle.SetTypeNameUID(resource_type_name_uid, original_name_uid);
		mOriginalResourceHandle.Init();
	}

	void ParamOverride::SetTextureOverride(u32 param_uid, u32 override_name_uid, Condition condition, u32 original_name_uid)
	{
		SetOverride(param_uid, RTYPE_iTexture, override_name_uid, condition, original_name_uid);
	}

#endif
}