#pragma once

namespace Render
{
	class DecalSettings : public UFG::qResourceData, public FXComponentSettings
	{
	public:
		CB_DecalSettings mCBSettings;
		u32 mTextureUID;
		u32 mNormalUID;
		u32 mShaderUID;
		u32 mAlphaStateUID;
		u32 mRasterStateUID;
		u32 mCount;
		f32 mEmissionRate;
		u32 mSpawnAgainOnLoop;
		f32 mRadiusMin;
		f32 mRadiusMax;
		f32 mDepth;
		f32 mFalloff;
		UFG::qVector3 mMinDir;
		UFG::qVector3 mMaxDir;
		u32 mDrawPass;
		int mNumPages;
		u32 mTextureOrientation;
		f32 mRotateMin;
		f32 mRotateMax;
		u32 mOnlyClipAlpha;
	};

	class DecalSettingsHandle : public UFG::qTypedResourceHandle<RTypeUID_DecalSettings, DecalSettings>
	{
	public:
	};
}