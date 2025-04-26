#pragma once

namespace UFG
{
	class RenderWorldState
	{
	public:
		static void Init();
	};

#ifdef THEORY_IMPL

	void RenderWorldState::Init()
	{
		Illusion::gStateSystem.AddParam("bbDeferredRendering", 0);
		Illusion::gStateSystem.AddParam("cbSphericalHarmonic", 0);
		Illusion::gStateSystem.AddParam("texDiffuseBlend", 1);
		Illusion::gStateSystem.AddParam("texEmission", 1);
		Illusion::gStateSystem.AddParam("texRealTimeEnvMap", 1);
		Illusion::gStateSystem.AddParam("texEnvMap", 1);
		Illusion::gStateSystem.AddParam("texFogCube", 1);
		Illusion::gStateSystem.AddParam("texNoise", 1);
		Illusion::gStateSystem.AddParam("texArcTan", 1);
		Illusion::gStateSystem.AddParam("sbDepthBiasSortLayer", 1);
		Illusion::gStateSystem.AddParam("cbWaterOffset", 0);
		Illusion::gStateSystem.AddParam("texNormal", 1);
		Illusion::gStateSystem.AddParam("texDepth", 1);
		Illusion::gStateSystem.AddParam("texDepth2", 1);
		Illusion::gStateSystem.AddParam("texCollector", 1);
		Illusion::gStateSystem.AddParam("cbMotionblurSettings", 0);
		Illusion::gStateSystem.AddParam("sbWindSettings", 1);
		Illusion::gStateSystem.AddParam("sbLitWindows", 1);
		Illusion::gStateSystem.AddParam("texReflection", 1);
		Illusion::gStateSystem.AddParam("texColourCube", 1);
		Illusion::gStateSystem.AddParam("cbEnvironmentSettings", 0);
		Illusion::gStateSystem.AddParam("texOcclusion", 1);
		Illusion::gStateSystem.AddParam("texWetSphericalMap", 1);
		Illusion::gStateSystem.AddParam("texIBLSphericalMap", 1);
		Illusion::gStateSystem.AddParam("texRipple", 1);
		Illusion::gStateSystem.AddParam("texWetLookMap", 1);
		Illusion::gStateSystem.AddParam("texShadowAtlas", 1);
		Illusion::gStateSystem.AddParam("texShadowRaw", 1);
		Illusion::gStateSystem.AddParam("texFadeDitherMask", 1);
		Illusion::gStateSystem.AddParam("cbShadowTransform", 0);
		Illusion::gStateSystem.AddParam("texDetailDiffuse0", 1);
		Illusion::gStateSystem.AddParam("texDetailNormal0", 1);
		Illusion::gStateSystem.AddParam("texDetailDiffuse1", 1);
		Illusion::gStateSystem.AddParam("texDetailNormal1", 1);
		Illusion::gStateSystem.AddParam("texDetailDiffuse2", 1);
		Illusion::gStateSystem.AddParam("texDetailNormal2", 1);
		Illusion::gStateSystem.AddParam("texFlowMap", 1);
		Illusion::gStateSystem.AddParam("texAmbientDiffuseMask", 1);

		// TODO: Implement RasterStates.
	}

#endif
}