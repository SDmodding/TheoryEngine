#pragma once

namespace Illusion
{
	class TextureLockInfo
	{
	public:
		char* mData;
		int mLinePitch;
		int mDepthPitch;
	};

	class TextureD3DResource : public UFG::qResourceData
	{
	public:
		D3D11_TEXTURE2D_DESC mDesc;
		ID3D11Resource* mGpuResource;
		ID3D11Resource* mStagingResource;
		u32 mStagingResourceInitialized;
		Illusion::TextureLockInfo* mDeferredContextTextureAccessBuffer;
		ID3D11ShaderResourceView* mShaderResourceView;
		u32 mNumberMipViews;
		ID3D11ShaderResourceView** mShaderResourceViewMips;
		ID3D11SamplerState* mSamplerState;
		ID3D11Resource* mDelayedReadCopy[5];
		int mRefCount;
		bool mAliased;
	};

	class TextureD3DResourceHandle : public UFG::qTypedResourceHandle<RTypeUID_TextureD3DResource, TextureD3DResource> {};

	class ITexturePlat
	{
	public:
		enum LockType
		{
			LOCK_NONE = -1,
			LOCK_READ = 0,
			LOCK_OVERWRITE,
			LOCK_MODIFY
		};
	};

	class TexturePlat
	{
	public:
		UFG::qOffset64<TextureUserPlat*> mTextureUserPlat;
		u32 mCpuAccess;
		u32 mMappedSubResourceIdx;
		u32 mUploadAfterUnmap;
		u32 mPad;
		char* mInitialImageData;
		ID3D11Resource* mMappedResource;
		UFG::qVRAMemoryHandle mVRamHandle;
		TextureD3DResourceHandle mTextureD3DResourceHandle;
		char mPadding[72];

		TexturePlat() : mCpuAccess(0), mMappedSubResourceIdx(0), mUploadAfterUnmap(0), mPad(0), mInitialImageData(0), mMappedResource(0)
		{
			UFG::qMemSet(mPadding, 0, sizeof(mPadding));
			mTextureD3DResourceHandle.mNameUID = 0;
		}
	};
}