#ifdef COMPILEPS

#ifndef D3D11

// D3D9 samplers
sampler2D sDiffMap : register(s0);
samplerCUBE sDiffCubeMap : register(s0);
sampler2D sAlbedoBuffer : register(s0);
sampler2D sMatalicMap : register(s1);
sampler2D sRoughnessMap : register(s2);
sampler2D sNormalMap : register(s3);
sampler2D sNormalBuffer : register(s3);
sampler2D sSpecMap : register(s4);
sampler2D sEmissiveMap : register(s5);
sampler2D sEnvMap : register(s6);
sampler3D sVolumeMap : register(s7);
samplerCUBE sEnvCubeMap : register(s6);
sampler2D sLightRampMap : register(s10);
sampler2D sLightSpotMap : register(s11);
samplerCUBE sLightCubeMap : register(s11);
sampler2D sShadowMap : register(s12);
samplerCUBE sFaceSelectCubeMap : register(s13);
samplerCUBE sIndirectionCubeMap : register(s14);
sampler2D sDepthBuffer : register(s16);
sampler2D sLightBuffer : register(s18);
samplerCUBE sZoneCubeMap : register(s17);
sampler3D sZoneVolumeMap : register(s17);

#define Sample2D(tex, uv) tex2D(s##tex, uv)
#define Sample2DProj(tex, uv) tex2Dproj(s##tex, uv)
#define Sample2DLod0(tex, uv) tex2Dlod(s##tex, float4(uv, 0.0, 0.0))
#define SampleCube(tex, uv) texCUBE(s##tex, uv)
#define SampleShadow(tex, uv) tex2Dproj(s##tex, uv)

#else

// D3D11 textures and samplers

Texture2D tDiffMap : register(t0);
TextureCube tDiffCubeMap : register(t0);
Texture2D tAlbedoBuffer : register(t0);
Texture2D tMatalicMap : register(t1);
Texture2D tRougnessMap : register(t2);
Texture2D tNormalMap : register(t3);
Texture2D tNormalBuffer : register(t3);
Texture2D tSpecMap : register(t4);
Texture2D tEmissiveMap : register(t5);
Texture2D tEnvMap : register(t6);
Texture3D tVolumeMap : register(t7);
TextureCube tEnvCubeMap : register(t6);
Texture2D tLightRampMap : register(t10);
Texture2D tLightSpotMap : register(t11);
TextureCube tLightCubeMap : register(t11);
Texture2D tShadowMap : register(t12);
TextureCube tFaceSelectCubeMap : register(t13);
TextureCube tIndirectionCubeMap : register(t14);
Texture2D tDepthBuffer : register(t15);
Texture2D tLightBuffer : register(t16);
TextureCube tZoneCubeMap : register(t17);
Texture3D tZoneVolumeMap : register(t17);

SamplerState sDiffMap : register(s0);
SamplerState sDiffCubeMap : register(s0);
SamplerState sAlbedoBuffer : register(s0);
SamplerState sMatalicMap : register(s1);
SamplerState sRougnessMap : register(s2);
SamplerState sNormalMap : register(s3);
SamplerState sNormalBuffer : register(s3);
SamplerState sSpecMap : register(s4);
SamplerState sEmissiveMap : register(s5);
SamplerState sEnvMap : register(s6);
SamplerState sVolumeMap : register(s7);
SamplerState sEnvCubeMap : register(s6);
SamplerState sLightRampMap : register(s10);
SamplerState sLightSpotMap : register(s11);
SamplerState sLightCubeMap : register(s11);
SamplerComparisonState sShadowMap : register(s12);
SamplerState sFaceSelectCubeMap : register(s13);
SamplerState sIndirectionCubeMap : register(s14);
SamplerState sDepthBuffer : register(s15);
SamplerState sLightBuffer : register(s16);
SamplerState sZoneCubeMap : register(s17);
SamplerState sZoneVolumeMap : register(s17);

#define Sample2D(tex, uv) t##tex.Sample(s##tex, uv)
#define Sample2DProj(tex, uv) t##tex.Sample(s##tex, uv.xy / uv.w)
#define Sample2DLod0(tex, uv) t##tex.SampleLevel(s##tex, uv, 0.0)
#define SampleCube(tex, uv) t##tex.Sample(s##tex, uv)
#define SampleShadow(tex, uv) t##tex.SampleCmpLevelZero(s##tex, uv.xy, uv.z)

#endif

float3 DecodeNormal(float4 normalInput)
{
#ifdef PACKEDNORMAL
    float3 normal;
    normal.xy = normalInput.ag * 2.0 - 1.0;
    normal.z = sqrt(max(1.0 - dot(normal.xy, normal.xy), 0.0));
    return normal;
#else
    return normalInput.rgb * 2.0 - 1.0;
#endif
}

float ReconstructDepth(float hwDepth)
{
    return dot(float2(hwDepth, cDepthReconstruct.y / (hwDepth - cDepthReconstruct.x)), cDepthReconstruct.zw);
}

#endif
