#pragma once

namespace Render
{
	void InitStateParams();

#ifdef THEORY_IMPL

	void InitStateParams()
	{
		Illusion::gStateSystem.AddParam("cbTimebase", 0);
		Illusion::gStateSystem.AddParam("cbColourBiasScale", 0);
		Illusion::gStateSystem.AddParam("cbShaderParams", 0);
		Illusion::gStateSystem.AddParam("cbViewTransform", 0);
		Illusion::gStateSystem.AddParam("texDiffuse", 1);
		Illusion::gStateSystem.AddParam("texBump", 1);
		Illusion::gStateSystem.AddParam("texSpecular", 1);
		Illusion::gStateSystem.AddParam("texEmissive", 1);
		Illusion::gStateSystem.AddParam("texDamage", 1);
		Illusion::gStateSystem.AddParam("texBlendMask", 1);
		Illusion::gStateSystem.AddParam("texDiffuse2", 1);
		Illusion::gStateSystem.AddParam("texBump2", 1);
		Illusion::gStateSystem.AddParam("texSpecular2", 1);
		Illusion::gStateSystem.AddParam("texDetail", 1);
		Illusion::gStateSystem.AddParam("texSphericalMap", 1);
		Illusion::gStateSystem.AddParam("texDistAtten", 1);
		Illusion::gStateSystem.AddParam("cbUberParams", 0);
		Illusion::gStateSystem.AddParam("cbLightSet", 0);
		Illusion::gStateSystem.AddParam("cbTextureCompColour", 0);
		Illusion::gStateSystem.AddParam("cbClipPlanes", 0);
		Illusion::gStateSystem.AddParam("cbDepthBiasSortLayer", 0);
		Illusion::gStateSystem.AddParam("cbSceneryInstance", 0);
	}

#endif
}