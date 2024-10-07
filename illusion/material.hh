#pragma once

namespace Illusion
{
	class MaterialParam : public StateParam
	{
	public:
		UFG::qGenericResourceHandle mResourceHandle;

		inline MaterialParam() 
		{
			mParamIndex = 0;
			pad0 = 0;
		}
	};

	class Material : public UFG::qResourceData
	{
	public:
		__declspec(align(16)) UFG::BitFlags128 mStateBlockMask;
		u32 mNumParams;
		UFG::qOffset64<MaterialUser*> mMaterialUser;

		Material();
		Material(const char* name, u32 name_uid, u32 num_params);

		THEORY_INLINE MaterialParam* GetParams() { return reinterpret_cast<MaterialParam*>(reinterpret_cast<uptr>(this) + sizeof(Material)); }
		THEORY_INLINE MaterialParam* GetParam(u32 index) { return &GetParams()[index]; }
	};

	class MaterialHandle : UFG::qTypedResourceHandle<RTYPE_Material, Material>
	{
	public:

	};

	class MaterialTable : public UFG::qResourceData
	{
	public:
		u32 mNumMaterials;
		UFG::qOffset64<MaterialTableUser*> mMaterialTableUser;
	};

#ifdef THEORY_IMPL

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

#endif
}