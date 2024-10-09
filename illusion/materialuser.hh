#pragma once

namespace Illusion
{
	class MaterialUser
	{
	public:
		s16 mVisibilityFlags;
		s16 mShadowFlags;

		MaterialUser() : mVisibilityFlags(0x1F), mShadowFlags(0) {}

		void OnLoad(Material* material);
	};

	class MaterialTableUser
	{
	public:
		int mExampleOfMaterialTableUserData1;
		float mExampleOfMaterialTableUserData2;
	};

#ifdef THEORY_IMPL

	void MaterialUser::OnLoad(Material* material)
	{
		if (!material->mNumParams) {
			return;
		}

		// TODO: Uncomment when 'AlphaState' & 'RasterState' defined.
		/*AlphaState* alphaState = nullptr;
		RasterState* rasterState = nullptr;

		for (u32 i = 0; material->mNumParams > i; ++i)
		{
			auto param = material->GetParam(i);
			if (param->mParamIndex == 9) { // AlphaState
				alphaState = param->mResourceHandle.GetData<AlphaState*>();
			}
			else if (param->mParamIndex == 10) { // RasterState
				rasterState = param->mResourceHandle.GetData<RasterState*>();
			}
		}

		if (alphaState && alphaState->mAlphaTestEnabled) {
			mShadowFlags |= 1;
		}

		if (rasterState && !rasterState->mCullMode) {
			mShadowFlags |= 2;
		}*/
	}

#endif
}