#pragma once

namespace Illusion
{
	class MaterialUser
	{
	public:
		s16 mVisibilityFlags;
		s16 mShadowFlags;

		inline MaterialUser() : mVisibilityFlags(0x1F), mShadowFlags(0) {}
	};

	class MaterialTableUser
	{
	public:
		int mExampleOfMaterialTableUserData1;
		float mExampleOfMaterialTableUserData2;
	};
}