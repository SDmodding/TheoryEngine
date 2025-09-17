#pragma once

namespace Render
{
	using namespace Illusion;

	class vCloudWispDescriptor : public VertexStreamDescriptor
	{
	public:
		vCloudWispDescriptor() : VertexStreamDescriptor("VertexDecl.CloudWisp", 0xE7CFFC61)
		{
			AddElement(VERTEX_ELEMENT_POSITION, VERTEX_TYPE_FLOAT4, 0);
			AddElement(VERTEX_ELEMENT_TEXCOORD0, VERTEX_TYPE_FLOAT4, 1);
			AddElement(VERTEX_ELEMENT_TEXCOORD1, VERTEX_TYPE_FLOAT4, 1);
			AddElement(VERTEX_ELEMENT_TEXCOORD2, VERTEX_TYPE_FLOAT4, 1);
			AddElement(VERTEX_ELEMENT_TEXCOORD3, VERTEX_TYPE_FLOAT4, 1);
		}
	} extern gCloudWispDescriptor;

	class vDecalDescriptor : public VertexStreamDescriptor
	{
	public:
		vDecalDescriptor() : VertexStreamDescriptor("VertexDecl.Decal", 0x128206B2)
		{
			AddElement(VERTEX_ELEMENT_POSITION, VERTEX_TYPE_FLOAT3, 0);
			AddElement(VERTEX_ELEMENT_TEXCOORD0, VERTEX_TYPE_HALF2, 0);
			AddElement(VERTEX_ELEMENT_NORMAL, VERTEX_TYPE_UBYTE4N, 0);
			AddElement(VERTEX_ELEMENT_TANGENT, VERTEX_TYPE_UBYTE4N, 0);
		}
	} extern gDecalDescriptor;

	class vDynamicDescriptor : public VertexStreamDescriptor
	{
	public:
		vDynamicDescriptor() : VertexStreamDescriptor("VertexDecl.Dynamic", 0x64A43DAE)
		{
			AddElement(VERTEX_ELEMENT_POSITION, VERTEX_TYPE_FLOAT3, 0);
			AddElement(VERTEX_ELEMENT_TEXCOORD0, VERTEX_TYPE_HALF2, 0);
			AddElement(VERTEX_ELEMENT_TEXCOORD1, VERTEX_TYPE_HALF2, 0);
			AddElement(VERTEX_ELEMENT_NORMAL, VERTEX_TYPE_FLOAT3, 0);
			AddElement(VERTEX_ELEMENT_TANGENT, VERTEX_TYPE_FLOAT3, 0);
			AddElement(VERTEX_ELEMENT_COLOR0, VERTEX_TYPE_COLOR4, 0);
		}
	} extern gDynamicDescriptor;

	class vFBXDescriptor : public VertexStreamDescriptor
	{
	public:
		vFBXDescriptor() : VertexStreamDescriptor("VertexDecl.FBX", 0xEE711E17)
		{
			AddElement(VERTEX_ELEMENT_POSITION, VERTEX_TYPE_FLOAT3, 0);
			AddElement(VERTEX_ELEMENT_TEXCOORD0, VERTEX_TYPE_HALF2, 1);
			AddElement(VERTEX_ELEMENT_TEXCOORD1, VERTEX_TYPE_HALF2, 1);
			AddElement(VERTEX_ELEMENT_NORMAL, VERTEX_TYPE_FLOAT3, 1);
			AddElement(VERTEX_ELEMENT_TANGENT, VERTEX_TYPE_FLOAT3, 1);
			AddElement(VERTEX_ELEMENT_COLOR0, VERTEX_TYPE_COLOR4, 1);
			AddElement(VERTEX_ELEMENT_BLENDINDEX, VERTEX_TYPE_UBYTE4, 1);
			AddElement(VERTEX_ELEMENT_BLENDWEIGHT, VERTEX_TYPE_UBYTE4N, 1);
		}
	} extern gFBXDescriptor;

	class vFBX2Descriptor : public VertexStreamDescriptor
	{
	public:
		vFBX2Descriptor() : VertexStreamDescriptor("VertexDecl.FBX2", 0x5589C78D)
		{
			AddElement(VERTEX_ELEMENT_POSITION, VERTEX_TYPE_FLOAT3, 0);
			AddElement(VERTEX_ELEMENT_TEXCOORD0, VERTEX_TYPE_HALF2, 1);
			AddElement(VERTEX_ELEMENT_TEXCOORD1, VERTEX_TYPE_HALF2, 1);
			AddElement(VERTEX_ELEMENT_COLOR0, VERTEX_TYPE_COLOR4, 2);
			AddElement(VERTEX_ELEMENT_NORMAL, VERTEX_TYPE_FLOAT3, 3);
			AddElement(VERTEX_ELEMENT_TANGENT, VERTEX_TYPE_FLOAT3, 3);
			AddElement(VERTEX_ELEMENT_BLENDINDEX, VERTEX_TYPE_UBYTE4, 3);
			AddElement(VERTEX_ELEMENT_BLENDWEIGHT, VERTEX_TYPE_UBYTE4N, 3);
		}
	} extern gFBX2Descriptor;

	class vGeoParticleDescriptor : public VertexStreamDescriptor
	{
	public:
		vGeoParticleDescriptor() : VertexStreamDescriptor("VertexDecl.GeoParticle", 0x25586179)
		{
			SetIndexStreamMultiplier(4);
			SetIndexStreamAsVertexStream();
			AddElement(VERTEX_ELEMENT_POSITION, VERTEX_TYPE_FLOAT3, 0);
			AddElement(VERTEX_ELEMENT_NORMAL, VERTEX_TYPE_FLOAT3, 0);
			AddElement(VERTEX_ELEMENT_TANGENT, VERTEX_TYPE_FLOAT3, 0);
			AddElement(VERTEX_ELEMENT_TEXCOORD0, VERTEX_TYPE_HALF2, 0);
			SetStreamRuntimeFlag(1);
			AddElement(VERTEX_ELEMENT_TEXCOORD1, VERTEX_TYPE_FLOAT4, 1);
			AddElement(VERTEX_ELEMENT_TEXCOORD2, VERTEX_TYPE_HALF4, 1);
			AddElement(VERTEX_ELEMENT_TEXCOORD3, VERTEX_TYPE_HALF4, 1);
			AddElement(VERTEX_ELEMENT_TEXCOORD4, VERTEX_TYPE_FLOAT4, 1);
			AddElement(VERTEX_ELEMENT_TEXCOORD5, VERTEX_TYPE_FLOAT3, 1);
			AddElement(VERTEX_ELEMENT_TEXCOORD6, VERTEX_TYPE_FLOAT3, 1);
			AddElement(VERTEX_ELEMENT_TEXCOORD7, VERTEX_TYPE_HALF4, 1);
			AddElement(VERTEX_ELEMENT_BLENDWEIGHT, VERTEX_TYPE_UBYTE4N, 1);
		}
	} extern gGeoParticleDescriptor;

	class vLightCardDescriptor : public VertexStreamDescriptor
	{
	public:
		vLightCardDescriptor() : VertexStreamDescriptor("VertexDecl.LightCard", 0x6FA1A436)
		{
			AddElement(VERTEX_ELEMENT_POSITION, VERTEX_TYPE_FLOAT3, 0);
			AddElement(VERTEX_ELEMENT_TEXCOORD1, VERTEX_TYPE_FLOAT4, 0);
			AddElement(VERTEX_ELEMENT_TEXCOORD0, VERTEX_TYPE_HALF2, 0);
			AddElement(VERTEX_ELEMENT_BLENDWEIGHT, VERTEX_TYPE_UBYTE4N, 0);
		}
	} extern gLightCardDescriptor;

	class vLightningDescriptor : public VertexStreamDescriptor
	{
	public:
		vLightningDescriptor() : VertexStreamDescriptor("VertexDecl.Lightning", 0x811776)
		{
			AddElement(VERTEX_ELEMENT_POSITION, VERTEX_TYPE_FLOAT3, 0);
			AddElement(VERTEX_ELEMENT_NORMAL, VERTEX_TYPE_FLOAT3, 0);
			AddElement(VERTEX_ELEMENT_TEXCOORD0, VERTEX_TYPE_HALF2, 0);
			AddElement(VERTEX_ELEMENT_COLOR0, VERTEX_TYPE_COLOR4, 0);
		}
	} extern gLightningDescriptor;

	class vMorphTargetDescriptor : public VertexStreamDescriptor
	{
	public:
		vMorphTargetDescriptor() : VertexStreamDescriptor("VertexDecl.MorphTarget", 0x7029F5BA)
		{
			AddElement(VERTEX_ELEMENT_POSITION, VERTEX_TYPE_UBYTE4_DELTA, 0);
			AddElement(VERTEX_ELEMENT_NORMAL, VERTEX_TYPE_BYTE4N, 0);
		}
	} extern gMorphTargetDescriptor;

	class vMultiMorphTargetDescriptor : public VertexStreamDescriptor
	{
	public:
		vMultiMorphTargetDescriptor() : VertexStreamDescriptor("VertexDecl.MultiMorphTarget", 0x6649E95)
		{
			AddElement(VERTEX_ELEMENT_POSITION, VERTEX_TYPE_SHORT4_FIXED4_12, 0);
			AddElement(VERTEX_ELEMENT_NORMAL, VERTEX_TYPE_BYTE4N, 0);
		}
	} extern gMultiMorphTargetDescriptor;

	class vNCDescriptor : public VertexStreamDescriptor
	{
	public:
		vNCDescriptor() : VertexStreamDescriptor("VertexDecl.NC", 0x34AB04E6)
		{
			UFG::qVector4 color = { 0.5f, 0.5f, 0.5f, 1.f };

			AddElement(VERTEX_ELEMENT_POSITION, VERTEX_TYPE_FLOAT3, 0, 0LL);
			AddElement(VERTEX_ELEMENT_NORMAL, VERTEX_TYPE_BYTE4N, 1, 0LL);
			AddElement(VERTEX_ELEMENT_COLOR0, VERTEX_TYPE_COLOR4, 1, &color);
		}
	} extern gNCDescriptor;

	class vParticleDescriptor : public VertexStreamDescriptor
	{
	public:
		vParticleDescriptor() : VertexStreamDescriptor("VertexDecl.Particle", 0xD7A5C4C7)
		{
			AddElement(VERTEX_ELEMENT_POSITION, VERTEX_TYPE_FLOAT4, 0);
			SetStreamRuntimeFlag(1);
			AddElement(VERTEX_ELEMENT_TEXCOORD0, VERTEX_TYPE_FLOAT4, 1);
			AddElement(VERTEX_ELEMENT_TEXCOORD1, VERTEX_TYPE_HALF4, 1);
			AddElement(VERTEX_ELEMENT_TEXCOORD2, VERTEX_TYPE_HALF4, 1);
			AddElement(VERTEX_ELEMENT_TEXCOORD3, VERTEX_TYPE_FLOAT4, 1);
			AddElement(VERTEX_ELEMENT_TEXCOORD4, VERTEX_TYPE_FLOAT3, 1);
			AddElement(VERTEX_ELEMENT_TEXCOORD5, VERTEX_TYPE_FLOAT3, 1);
			AddElement(VERTEX_ELEMENT_TEXCOORD6, VERTEX_TYPE_HALF4, 1);
			AddElement(VERTEX_ELEMENT_BLENDWEIGHT, VERTEX_TYPE_UBYTE4N, 1);
		}
	} extern gParticleDescriptor;

	class vRainDescriptor : public VertexStreamDescriptor
	{
	public:
		vRainDescriptor() : VertexStreamDescriptor("VertexDecl.Rain", 0x48649FC8)
		{
			AddElement(VERTEX_ELEMENT_POSITION, VERTEX_TYPE_FLOAT3, 0);
			AddElement(VERTEX_ELEMENT_TEXCOORD0, VERTEX_TYPE_HALF2, 0);
		}
	} extern gRainDescriptor;

	class vScreenParticleDescriptor : public VertexStreamDescriptor
	{
	public:
		vScreenParticleDescriptor() : VertexStreamDescriptor("VertexDecl.ScreenParticle", 0x3D339245)
		{
			AddElement(VERTEX_ELEMENT_POSITION, VERTEX_TYPE_FLOAT3, 0);
			AddElement(VERTEX_ELEMENT_TEXCOORD0, VERTEX_TYPE_HALF2, 0);
			AddElement(VERTEX_ELEMENT_TEXCOORD1, VERTEX_TYPE_FLOAT4, 0);
		}
	} extern gScreenParticleDescriptor;

	class vSkinnedDescriptor : public VertexStreamDescriptor
	{
	public:
		vSkinnedDescriptor() : VertexStreamDescriptor("VertexDecl.Skinned", 0x276B9567)
		{
			AddElement(VERTEX_ELEMENT_POSITION, VERTEX_TYPE_FLOAT4, 0);
			AddElement(VERTEX_ELEMENT_NORMAL, VERTEX_TYPE_BYTE4N, 0);
			AddElement(VERTEX_ELEMENT_TANGENT, VERTEX_TYPE_BYTE4N, 0);
			AddElement(VERTEX_ELEMENT_BLENDINDEX, VERTEX_TYPE_UBYTE4, 1);
			AddElement(VERTEX_ELEMENT_BLENDWEIGHT, VERTEX_TYPE_UBYTE4N, 1);
			AddElement(VERTEX_ELEMENT_TEXCOORD0, VERTEX_TYPE_HALF2, 2);
		}

		void ApplyUserRemapping(UFG::qVector4* vertex_element) override
		{
			f32 x = vertex_element[3].x;
			if (x <= 0.19611764f) { // what is this magic number?
				x = 0.19611764f;
			}
			else if (x >= 1.f)
			{
				vertex_element[2].w = 1.f * vertex_element[2].w;
				return;
			}

			vertex_element[2].w = x * vertex_element[2].w;
		}

	} extern gSkinnedDescriptor;

	class vSkinnedDecalDescriptor : public VertexStreamDescriptor
	{
	public:
		vSkinnedDecalDescriptor() : VertexStreamDescriptor("VertexDecl.SkinnedDecal", 0x23F36527)
		{
			AddElement(VERTEX_ELEMENT_POSITION, VERTEX_TYPE_FLOAT4, 0);
			AddElement(VERTEX_ELEMENT_NORMAL, VERTEX_TYPE_BYTE4N, 0);
			AddElement(VERTEX_ELEMENT_TANGENT, VERTEX_TYPE_BYTE4N, 0);
			AddElement(VERTEX_ELEMENT_TEXCOORD0, VERTEX_TYPE_HALF2, 0);
		}
	} extern gSkinnedDecalDescriptor;

	class vSkinnedUVNTDescriptor : public VertexStreamDescriptor
	{
	public:
		vSkinnedUVNTDescriptor() : VertexStreamDescriptor("VertexDecl.SkinnedUVNT", 0xAC5D89E2)
		{
			AddElement(VERTEX_ELEMENT_POSITION, VERTEX_TYPE_FLOAT4, 0);
			AddElement(VERTEX_ELEMENT_NORMAL, VERTEX_TYPE_BYTE4N, 0);
			AddElement(VERTEX_ELEMENT_TANGENT, VERTEX_TYPE_BYTE4N, 0);
			AddElement(VERTEX_ELEMENT_TEXCOORD0, VERTEX_TYPE_HALF2, 0);
		}
	} extern gSkinnedUVNTDescriptor;

	class vSlimDescriptor : public VertexStreamDescriptor
	{
	public:
		vSlimDescriptor() : VertexStreamDescriptor("VertexDecl.Slim", 0x91355905)
		{
			AddElement(VERTEX_ELEMENT_POSITION, VERTEX_TYPE_FLOAT3, 0);
		}
	} extern gSlimDescriptor;

	class vSlimUVDescriptor : public VertexStreamDescriptor
	{
	public:
		vSlimUVDescriptor() : VertexStreamDescriptor("VertexDecl.SlimUV", 0x7E0D7533)
		{
			AddElement(VERTEX_ELEMENT_POSITION, VERTEX_TYPE_FLOAT3, 0);
			AddElement(VERTEX_ELEMENT_TEXCOORD0, VERTEX_TYPE_HALF2, 1);
		}
	} extern gSlimUVDescriptor;

	class vUV2Descriptor : public VertexStreamDescriptor
	{
	public:
		vUV2Descriptor() : VertexStreamDescriptor("VertexDecl.UV2", 0x27C88915)
		{
			AddElement(VERTEX_ELEMENT_POSITION, VERTEX_TYPE_FLOAT3, 0);
			AddElement(VERTEX_ELEMENT_TEXCOORD0, VERTEX_TYPE_HALF2, 1);
			AddElement(VERTEX_ELEMENT_TEXCOORD1, VERTEX_TYPE_HALF2, 1);
		}
	} extern gUV2Descriptor;

	class vUV2CDescriptor : public VertexStreamDescriptor
	{
	public:
		vUV2CDescriptor() : VertexStreamDescriptor("VertexDecl.UV2C", 0x7728A54B)
		{
			AddElement(VERTEX_ELEMENT_POSITION, VERTEX_TYPE_FLOAT3, 0);
			AddElement(VERTEX_ELEMENT_TEXCOORD0, VERTEX_TYPE_HALF2, 1);
			AddElement(VERTEX_ELEMENT_TEXCOORD1, VERTEX_TYPE_HALF2, 1);
			AddElement(VERTEX_ELEMENT_COLOR0, VERTEX_TYPE_COLOR4, 1);
		}
	} extern gUV2CDescriptor;

	class vUV2NDescriptor : public VertexStreamDescriptor
	{
	public:
		vUV2NDescriptor() : VertexStreamDescriptor("VertexDecl.UV2N", 0x46E52398)
		{
			AddElement(VERTEX_ELEMENT_POSITION, VERTEX_TYPE_FLOAT3, 0);
			AddElement(VERTEX_ELEMENT_TEXCOORD0, VERTEX_TYPE_HALF2, 1);
			AddElement(VERTEX_ELEMENT_TEXCOORD1, VERTEX_TYPE_HALF2, 1);
			AddElement(VERTEX_ELEMENT_NORMAL, VERTEX_TYPE_BYTE4N, 1);
		}
	} extern gUV2NDescriptor;

	class vUV2NCDescriptor : public VertexStreamDescriptor
	{
	public:
		vUV2NCDescriptor() : VertexStreamDescriptor("VertexDecl.UV2NC", 0xF2E6F36B)
		{
			AddElement(VERTEX_ELEMENT_POSITION, VERTEX_TYPE_FLOAT3, 0);
			AddElement(VERTEX_ELEMENT_TEXCOORD0, VERTEX_TYPE_HALF2, 1);
			AddElement(VERTEX_ELEMENT_TEXCOORD1, VERTEX_TYPE_HALF2, 1);
			AddElement(VERTEX_ELEMENT_NORMAL, VERTEX_TYPE_BYTE4N, 1);
			AddElement(VERTEX_ELEMENT_COLOR0, VERTEX_TYPE_COLOR4, 1);
		}
	} extern gUV2NCDescriptor;

	class vUV2NTDescriptor : public VertexStreamDescriptor
	{
	public:
		vUV2NTDescriptor() : VertexStreamDescriptor("VertexDecl.UV2NT", 0xA0B0781E)
		{
			AddElement(VERTEX_ELEMENT_POSITION, VERTEX_TYPE_FLOAT3, 0);
			AddElement(VERTEX_ELEMENT_TEXCOORD0, VERTEX_TYPE_HALF2, 1);
			AddElement(VERTEX_ELEMENT_TEXCOORD1, VERTEX_TYPE_HALF2, 1);
			AddElement(VERTEX_ELEMENT_NORMAL, VERTEX_TYPE_BYTE4N, 1);
			AddElement(VERTEX_ELEMENT_TANGENT, VERTEX_TYPE_BYTE4N, 1);
		}
	} extern gUV2NTDescriptor;

	class vUV2NTCDescriptor : public VertexStreamDescriptor
	{
	public:
		vUV2NTCDescriptor() : VertexStreamDescriptor("VertexDecl.UV2NTC", 0x78921EA0)
		{
			UFG::qVector4 color = { 0.f, 0.f, 0.f, 1.f };

			AddElement(VERTEX_ELEMENT_POSITION, VERTEX_TYPE_FLOAT3, 0);
			AddElement(VERTEX_ELEMENT_TEXCOORD0, VERTEX_TYPE_HALF2, 1);
			AddElement(VERTEX_ELEMENT_TEXCOORD1, VERTEX_TYPE_HALF2, 1);
			AddElement(VERTEX_ELEMENT_NORMAL, VERTEX_TYPE_BYTE4N, 1);
			AddElement(VERTEX_ELEMENT_TANGENT, VERTEX_TYPE_BYTE4N, 1);
			AddElement(VERTEX_ELEMENT_COLOR0, VERTEX_TYPE_COLOR4, 1, &color);
		}

		void ApplyUserRemapping(UFG::qVector4* vertex_element) override
		{
			vertex_element[1].w = vertex_element[3].w;
		}

	} extern gUV2NTCDescriptor;

	class vUVCDescriptor : public VertexStreamDescriptor
	{
	public:
		vUVCDescriptor() : VertexStreamDescriptor("VertexDecl.UVC", 0xC3BD8945)
		{
			AddElement(VERTEX_ELEMENT_POSITION, VERTEX_TYPE_FLOAT3, 0);
			AddElement(VERTEX_ELEMENT_TEXCOORD0, VERTEX_TYPE_HALF2, 1);
			AddElement(VERTEX_ELEMENT_COLOR0, VERTEX_TYPE_COLOR4, 1);
		}
	} extern gUVCDescriptor;

	class vUVCNPackedDescriptor : public VertexStreamDescriptor
	{
	public:
		vUVCNPackedDescriptor() : VertexStreamDescriptor("VertexDecl.UVCNPacked", 0xF0D9B822)
		{
			AddElement(VERTEX_ELEMENT_POSITION, VERTEX_TYPE_HALF4, 0);
			AddElement(VERTEX_ELEMENT_TEXCOORD0, VERTEX_TYPE_HALF2, 1);
			AddElement(VERTEX_ELEMENT_NORMAL, VERTEX_TYPE_BYTE4N, 1);
			AddElement(VERTEX_ELEMENT_COLOR0, VERTEX_TYPE_COLOR4, 1);
		}
		
		void ApplyUserRemapping(UFG::qVector4* vertex_element) override
		{
			vertex_element->w = 1.0;
		}

	} extern gUVCNPackedDescriptor;

	class vUVNDescriptor : public VertexStreamDescriptor
	{
	public:
		vUVNDescriptor() : VertexStreamDescriptor("VertexDecl.UVN", 0xF2700F96)
		{
			AddElement(VERTEX_ELEMENT_POSITION, VERTEX_TYPE_FLOAT3, 0);
			AddElement(VERTEX_ELEMENT_TEXCOORD0, VERTEX_TYPE_HALF2, 1);
			AddElement(VERTEX_ELEMENT_NORMAL, VERTEX_TYPE_BYTE4N, 1);
		}
	} extern gUVNDescriptor;

	class vUVNPackedDescriptor : public VertexStreamDescriptor
	{
	public:
		vUVNPackedDescriptor() : VertexStreamDescriptor("VertexDecl.UVNPacked", 0x720788F7)
		{
			AddElement(VERTEX_ELEMENT_POSITION, VERTEX_TYPE_HALF4, 0);
			AddElement(VERTEX_ELEMENT_TEXCOORD0, VERTEX_TYPE_HALF2, 1);
			AddElement(VERTEX_ELEMENT_NORMAL, VERTEX_TYPE_BYTE4N, 1);
		}

		void ApplyUserRemapping(UFG::qVector4* vertex_element) override
		{
			vertex_element->w = 1.0;
		}

	} extern gUVNPackedDescriptor;

	class vUVNTDescriptor : public VertexStreamDescriptor
	{
	public:
		vUVNTDescriptor() : VertexStreamDescriptor("VertexDecl.UVNT", 0x9BA68DBC)
		{
			AddElement(VERTEX_ELEMENT_POSITION, VERTEX_TYPE_FLOAT3, 0);
			AddElement(VERTEX_ELEMENT_TEXCOORD0, VERTEX_TYPE_HALF2, 1);
			AddElement(VERTEX_ELEMENT_NORMAL, VERTEX_TYPE_BYTE4N, 1);
			AddElement(VERTEX_ELEMENT_TANGENT, VERTEX_TYPE_BYTE4N, 1);
		}
	} extern gUVNTDescriptor;

	class vUVNTCDescriptor : public VertexStreamDescriptor
	{
	public:
		vUVNTCDescriptor() : VertexStreamDescriptor("VertexDecl.UVNTC", 0x911E1A51)
		{
			UFG::qVector4 color = { 0.f, 0.f, 0.f, 1.f };

			AddElement(VERTEX_ELEMENT_POSITION, VERTEX_TYPE_FLOAT3, 0);
			AddElement(VERTEX_ELEMENT_TEXCOORD0, VERTEX_TYPE_HALF2, 1);
			AddElement(VERTEX_ELEMENT_NORMAL, VERTEX_TYPE_BYTE4N, 1);
			AddElement(VERTEX_ELEMENT_TANGENT, VERTEX_TYPE_BYTE4N, 1);
			AddElement(VERTEX_ELEMENT_COLOR0, VERTEX_TYPE_COLOR4, 1, &color);
		}
	} extern gUVNTCDescriptor;

	class vVehicleGlassUV2NTCDescriptor : public VertexStreamDescriptor
	{
	public:
		vVehicleGlassUV2NTCDescriptor() : VertexStreamDescriptor("VertexDecl.VehicleGlassUV2NTC", 0xD08FF31A)
		{
			AddElement(VERTEX_ELEMENT_POSITION, VERTEX_TYPE_FLOAT4, 0);
			AddElement(VERTEX_ELEMENT_NORMAL, VERTEX_TYPE_BYTE4N, 0);
			AddElement(VERTEX_ELEMENT_TANGENT, VERTEX_TYPE_BYTE4N, 0);
			AddElement(VERTEX_ELEMENT_BLENDINDEX, VERTEX_TYPE_UBYTE4, 1);
			AddElement(VERTEX_ELEMENT_BLENDWEIGHT, VERTEX_TYPE_UBYTE4N, 1);
			AddElement(VERTEX_ELEMENT_TEXCOORD0, VERTEX_TYPE_HALF2, 2);
			AddElement(VERTEX_ELEMENT_TEXCOORD1, VERTEX_TYPE_HALF2, 2);
			AddElement(VERTEX_ELEMENT_COLOR0, VERTEX_TYPE_COLOR4, 2);
		}
	} extern gVehicleGlassUV2NTCDescriptor;

	class vVehicleUV2NTDescriptor : public VertexStreamDescriptor
	{
	public:
		vVehicleUV2NTDescriptor() : VertexStreamDescriptor("VertexDecl.VehicleUV2NTC", 0xBF900E9E)
		{
			AddElement(VERTEX_ELEMENT_POSITION, VERTEX_TYPE_FLOAT4, 0);
			AddElement(VERTEX_ELEMENT_NORMAL, VERTEX_TYPE_BYTE4N, 0);
			AddElement(VERTEX_ELEMENT_TANGENT, VERTEX_TYPE_BYTE4N, 0);
			AddElement(VERTEX_ELEMENT_BLENDINDEX, VERTEX_TYPE_UBYTE4, 1);
			AddElement(VERTEX_ELEMENT_BLENDWEIGHT, VERTEX_TYPE_UBYTE4N, 1);
			AddElement(VERTEX_ELEMENT_TEXCOORD0, VERTEX_TYPE_HALF2, 2);
			AddElement(VERTEX_ELEMENT_TEXCOORD1, VERTEX_TYPE_HALF2, 2);
		}
	} extern gVehicleUV2NTDescriptor;

	class vVehicleUVNTDescriptor : public VertexStreamDescriptor
	{
	public:
		vVehicleUVNTDescriptor() : VertexStreamDescriptor("VertexDecl.VehicleUVNTC", 0xE234EF7A)
		{
			AddElement(VERTEX_ELEMENT_POSITION, VERTEX_TYPE_FLOAT4, 0);
			AddElement(VERTEX_ELEMENT_NORMAL, VERTEX_TYPE_BYTE4N, 0);
			AddElement(VERTEX_ELEMENT_TANGENT, VERTEX_TYPE_BYTE4N, 0);
			AddElement(VERTEX_ELEMENT_BLENDINDEX, VERTEX_TYPE_UBYTE4, 1);
			AddElement(VERTEX_ELEMENT_BLENDWEIGHT, VERTEX_TYPE_UBYTE4N, 1);
			AddElement(VERTEX_ELEMENT_TEXCOORD0, VERTEX_TYPE_HALF2, 2);
		}
	} extern gVehicleUVNTDescriptor;

	class vWaterSurfaceDescriptor : public VertexStreamDescriptor
	{
	public:
		vWaterSurfaceDescriptor() : VertexStreamDescriptor("VertexDecl.WaterSurface", 0xD3798DFD)
		{
			AddElement(VERTEX_ELEMENT_POSITION, VERTEX_TYPE_FLOAT4, 0);
			AddElement(VERTEX_ELEMENT_NORMAL, VERTEX_TYPE_FLOAT4, 0);
			AddElement(VERTEX_ELEMENT_TEXCOORD0, VERTEX_TYPE_HALF2, 1);
		}
	} extern gWaterSurfaceDescriptor;

#ifdef THEORY_IMPL

	vCloudWispDescriptor gCloudWispDescriptor;
	vDecalDescriptor gDecalDescriptor;
	vDynamicDescriptor gDynamicDescriptor;
	vFBXDescriptor gFBXDescriptor;
	vFBX2Descriptor gFBX2Descriptor;
	vGeoParticleDescriptor gGeoParticleDescriptor;
	vLightCardDescriptor gLightCardDescriptor;
	vLightningDescriptor gLightningDescriptor;
	vMorphTargetDescriptor gMorphTargetDescriptor;
	vMultiMorphTargetDescriptor gMultiMorphTargetDescriptor;
	vNCDescriptor gNCDescriptor;
	vParticleDescriptor gParticleDescriptor;
	vRainDescriptor gRainDescriptor;
	vScreenParticleDescriptor gScreenParticleDescriptor;
	vSkinnedDescriptor gSkinnedDescriptor;
	vSkinnedDecalDescriptor gSkinnedDecalDescriptor;
	vSkinnedUVNTDescriptor gSkinnedUVNTDescriptor;
	vSlimDescriptor gSlimDescriptor;
	vSlimUVDescriptor gSlimUVDescriptor;
	vUV2Descriptor gUV2Descriptor;
	vUV2CDescriptor gUV2CDescriptor;
	vUV2NDescriptor gUV2NDescriptor;
	vUV2NCDescriptor gUV2NCDescriptor;
	vUV2NTDescriptor gUV2NTDescriptor;
	vUV2NTCDescriptor gUV2NTCDescriptor;
	vUVCDescriptor gUVCDescriptor;
	vUVCNPackedDescriptor gUVCNPackedDescriptor;
	vUVNDescriptor gUVNDescriptor;
	vUVNPackedDescriptor gUVNPackedDescriptor;
	vUVNTDescriptor gUVNTDescriptor;
	vUVNTCDescriptor gUVNTCDescriptor;
	vVehicleGlassUV2NTCDescriptor gVehicleGlassUV2NTCDescriptor;
	vVehicleUV2NTDescriptor gVehicleUV2NTDescriptor;
	vVehicleUVNTDescriptor gVehicleUVNTDescriptor;
	vWaterSurfaceDescriptor gWaterSurfaceDescriptor;

#endif
}