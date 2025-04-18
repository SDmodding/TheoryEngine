#pragma once

namespace UFG
{
	class PedSpawningInfo;
	class TrueCrowdModelPart;
	class TrueCrowdTextureSet;
	class TextureOverrideParams;

	enum eTrueCrowdMorphType
	{
		eTCMT_None,
		eTCMT_Base,
		eTCMT_Target1,
		eTCMT_Target2
	};

	struct ModelTextureCombination
	{
		u32 mModelIndex;
		u32 mTextureSetIndex;
		qColour* mColourTint;
	};

	class TrueCrowdModelPart
	{
	public:
		qOffset64<char*> mModelName;
		u32 mModelNameHash;
		u16 mIsSkinned;
		qEnum<eTrueCrowdMorphType, s16> mMorphType;
	};

	class TrueCrowdLOD
	{
	public:
		u32 mNumModelParts;
		qOffset64<TrueCrowdModelPart*> mModelParts;
	};

	class TrueCrowdResource
	{
	public:
		enum EntityType : s32
		{
			Invalid = -1,
			Character = 0,
			Vehicle,
			Prop,
			NumEntityTypes,
		};

		qOffset64<char*> mName;
		ResourceRequest* mRequest;
		u32 mModelSize;
		u32 mTextureSize;
		qEnum<EntityType, u32> mType;
		qSymbol mPathSymbol;
		qSymbol mPropSetName;
		u32 mNonDefraggable;
		qPropertySet* mPropSet;
		qOffset64<TrueCrowdResource*> mHighResolutionResource;
	};

	class TrueCrowdModel : public TrueCrowdResource
	{
	public:
		u16 mNumLODs;
		u16 mNumTextureSets;
		u32 mComponentTypeSymbolUC;
		qOffset64<TrueCrowdLOD*> mLODModel;
		qOffset64<qOffset64<TrueCrowdTextureSet*>*> mTextureSets;
	};

	class TextureOverrideParams
	{
	public:
		enum eSampler
		{
			eDiffuse = 0xDCE06689,
			eDiffuse2 = 0x19410F73,
			eNormal = 0xADBE1A5A,
			eSpecular = 0xCB460EC7,
			eSelfIllumination = 0xBEFFB144
		};

		qEnum<eSampler, u32> mSampler;
		u32 mTextureNameUID;
		u32 mTextureOverrideUID[3];
		u32 mPad0;
	};

	class TrueCrowdTextureSet : public TrueCrowdResource
	{
	public:
		qOffset64<qColour*> mColourTints;
		qOffset64<TextureOverrideParams*> mTextureOverrideParams;
		u32 mNumColorTints;
		u16 mNumTextureOverrideParams;
		u16 mTintIndex;
	};

	class TrueCrowdSet : public qNode<TrueCrowdSet>
	{
	public:
		struct ComponentDetails
		{
			s8 mbSpecificModel;
			s8 mbTextureSetIndexValid;
			s8 mbColourTintIndexValid;
			u32 mTextureSetIndex;
			u32 mColourTintIndex;
		};

		struct Instance
		{
			TrueCrowdSet* mSet;
			u32 mNumParts;
			f32 mMorphWeights[2];
			ModelTextureCombination mPart[16];
		};

		BitFlags128 mKey;
		qSymbol mResourceFilename;
		u32 mEntityIndex;
		u32 mCurrentInstances;
		qArray<TrueCrowdModel*> mFiles[16];
		ComponentDetails mComponentDetails[16];
	};

	class TrueCrowdDataBase : public qResourceData
	{
	public:
		struct ResourceEntry
		{
			BitFlags128 mTagBitFlag;
			TrueCrowdModel mResource;
		};

		struct ComponentEntries
		{
			u32 mNumEntries;
			qOffset64<ResourceEntry*> mEntries;
		};

		TrueCrowdDefinition mDefinition;
		u32 mNumComponentEntries;
		qOffset64<ComponentEntries*> mComponentEntries;
		qList<TrueCrowdSet> mKnownRequests;

		THEORY_SINLINE TrueCrowdDataBase* sTrueCrowdDataBase = 0;

		void Init() { StreamResourceLoader::LoadResourceFile(R"(Data\Global\TrueCrowd.bin)"); }
	};
}