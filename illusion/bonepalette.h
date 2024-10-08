#pragma once

namespace Illusion
{
	class BonePalette : public UFG::qResourceData
	{
	public:
		u32 mNumPalettes;
		u32 mNumBones;
		UFG::qOffset64<char*> mBoneNameTable;
		UFG::qOffset64<u32*> mBoneUIDTable;
		UFG::qOffset64<u32*> mBoneFullUIDTable;
	};

	class BonePaletteHandle : public UFG::qTypedResourceHandle<RTYPE_BonePalette, BonePalette>
	{
	public:
	};

}