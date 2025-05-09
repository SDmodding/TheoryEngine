#pragma once

namespace Render
{
	class BeamSettings : public UFG::qResourceData, public FXComponentSettings
	{
	public:
		UFG::qVector4 minColour;
		UFG::qVector4 maxColour;
		UFG::qVector2 numAnimPages;
		f32 mWidth;
		f32 mLength;
		u32 mTextureUID;
		f32 mLifeTime;
		u32 mRayCastLength;
	};

	class BeamSettingsHandle : public UFG::qTypedResourceHandle<RTypeUID_BeamSettings, BeamSettings>
	{
	public:
	};
}