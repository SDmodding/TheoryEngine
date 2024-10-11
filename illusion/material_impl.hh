#pragma once

#ifdef THEORY_IMPL
namespace Illusion
{
	Material::Material() : UFG::qResourceData()
	{
		for (u32 i = 0; mNumParams > i; ++i) {
			new (GetParam(i)) MaterialParam;
		}

		mStateBlockMask.Reset();
	}

	Material::Material(const char* name, u32 name_uid, u32 num_params) : UFG::qResourceData(RTYPE_Material, name_uid, name)
	{
		mNumParams = num_params;

		for (u32 i = 0; mNumParams > i; ++i) {
			new (GetParam(i)) MaterialParam;
		}

		mStateBlockMask.Reset();
	}

	void Material::OnLoad()
	{
		new (this) Material;

		for (u32 i = 0; mNumParams > i; ++i)
		{
			auto param = GetParam(i);
			param->mParamIndex = GetStateSystem()->AddParam(param->mNameUID, nullptr, false);

			if (param->mResourceHandle.mNameUID) {
				param->mResourceHandle.Init();
			}

			mStateBlockMask.Set(static_cast<u64>(param->mParamIndex));
		}

		mMaterialUser->OnLoad(this);
	}

	void Material::OnUnload()
	{
		for (u32 i = 0; mNumParams > i; ++i)
		{
			auto param = GetParam(i);
			if (param->mResourceHandle.mNameUID) {
				param->mResourceHandle.Close();
			}
		}
	}
}
#endif