#pragma once

namespace Illusion
{
	class MaterialUser;
	class MaterialTableUser;

	class MaterialParam : public StateParam
	{
	public:
		UFG::qGenericResourceHandle mResourceHandle;

		MaterialParam() 
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
		UFG::qOffset<MaterialUser*> mMaterialUser;

		Material();
		Material(const char* name, u32 name_uid, u32 num_params);

		void OnLoad();
		void OnUnload();

		THEORY_INLINE MaterialParam* GetParams() { return reinterpret_cast<MaterialParam*>(&this[1]); }
		THEORY_INLINE MaterialParam* GetParam(u32 index) { return &GetParams()[index]; }
	};

	class MaterialHandle : UFG::qTypedResourceHandle<RTypeUID_Material, Material>
	{
	public:

	};

	class MaterialTable : public UFG::qResourceData
	{
	public:
		u32 mNumMaterials;
		UFG::qOffset<MaterialTableUser*> mMaterialTableUser;
	};

	class MaterialTableHandle : public UFG::qTypedResourceHandle<RTypeUID_MaterialTable, MaterialTable>
	{
	public:
	};

}