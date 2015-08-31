#include "Uniforms.hlsl"
#include "Samplers.hlsl"
#include "Transform.hlsl"
#include "ScreenPos.hlsl"
#include "Lighting.hlsl"
#include "BRDF.hlsl"
#include "DeferredGBuffer.hlsl"

void VS(float4 iPos : POSITION,
    #ifdef DIRLIGHT
        out float2 oScreenPos : TEXCOORD0,
    #else
        out float4 oScreenPos : TEXCOORD0,
    #endif
    out float3 oFarRay : TEXCOORD1,
    #ifdef ORTHO
        out float3 oNearRay : TEXCOORD2,
    #endif
    out float4 oPos : OUTPOSITION)
{
    float4x3 modelMatrix = iModelMatrix;
    float3 worldPos = GetWorldPos(modelMatrix);
    oPos = GetClipPos(worldPos);
    #ifdef DIRLIGHT
        oScreenPos = GetScreenPosPreDiv(oPos);
        oFarRay = GetFarRay(oPos);
        #ifdef ORTHO
            oNearRay = GetNearRay(oPos);
        #endif
    #else
        oScreenPos = GetScreenPos(oPos);
        oFarRay = GetFarRay(oPos) * oPos.w;
        #ifdef ORTHO
            oNearRay = GetNearRay(oPos) * oPos.w;
        #endif
    #endif
}

void PS(
    #ifdef DIRLIGHT
        float2 iScreenPos : TEXCOORD0,
    #else
        float4 iScreenPos : TEXCOORD0,
    #endif
    float3 iFarRay : TEXCOORD1,
    #ifdef ORTHO
        float3 iNearRay : TEXCOORD2,
    #endif
    #ifdef PBR
        float2 iFragPos : VPOS,
    #endif
    out float4 oColor : OUTCOLOR0)
{
    // If rendering a directional light quad, optimize out the w divide
    #ifdef DIRLIGHT
        float depth = Sample2DLod0(DepthBuffer, iScreenPos).r;
        #ifdef HWDEPTH
            depth = ReconstructDepth(depth);
        #endif
        #ifdef ORTHO
            float3 worldPos = lerp(iNearRay, iFarRay, depth);
        #else
            float3 worldPos = iFarRay * depth;
        #endif
        float4 albedoInput = Sample2DLod0(AlbedoBuffer, iScreenPos);
        float4 normalInput = Sample2DLod0(NormalBuffer, iScreenPos);
    #else
        float depth = Sample2DProj(DepthBuffer, iScreenPos).r;
        #ifdef HWDEPTH
            depth = ReconstructDepth(depth);
        #endif
        #ifdef ORTHO
            float3 worldPos = lerp(iNearRay, iFarRay, depth) / iScreenPos.w;
        #else
            float3 worldPos = iFarRay * depth / iScreenPos.w;
        #endif
        float4 albedoInput = Sample2DProj(AlbedoBuffer, iScreenPos);
        float4 normalInput = Sample2DProj(NormalBuffer, iScreenPos);
    #endif
    
    #ifdef PBR
        float3 normal = DecodeGBufferNormal(normalInput.xy, iFarRay);
        const float roughness = normalInput.b;
        #ifdef DIRLIGHT
            float3 specColor = 0;
            float3 albedoColor = 0;
            DecodeYCoCgElements(iScreenPos.xy, albedoInput, albedoColor, specColor);
            albedoInput.rgb = albedoColor;
            albedoInput.a = 1;
        #else
            float3 specColor = 0;
            float3 albedoColor = 0;
            DecodeYCoCgElements(iScreenPos.xy/iScreenPos.w, albedoInput, albedoColor, specColor);
            albedoInput.rgb = albedoColor;
            albedoInput.a = 1;
        #endif
        albedoInput.a = 1.0;
    #else
        float3 normal = normalize(normalInput.rgb * 2.0 - 1.0);
    #endif
    
    float4 projWorldPos = float4(worldPos, 1.0);
    float3 lightColor;
    float3 lightDir;

    float diff = GetDiffuse(normal, worldPos, lightDir);

    #ifdef SHADOW
        diff *= GetShadowDeferred(projWorldPos, depth);
    #endif

    #if defined(SPOTLIGHT)
        float4 spotPos = mul(projWorldPos, cLightMatricesPS[0]);
        lightColor = spotPos.w > 0.0 ? Sample2DProj(LightSpotMap, spotPos).rgb * cLightColor.rgb : 0.0;
    #elif defined(CUBEMASK)
        lightColor = texCUBE(sLightCubeMap, mul(worldPos - cLightPosPS.xyz, (float3x3)cLightMatricesPS[0])).rgb * cLightColor.rgb;
    #else
        lightColor = cLightColor.rgb;
    #endif

    #ifdef PBR          
        float3 toCamera = normalize(-worldPos);
                
        const float3 Hn = normalize(toCamera + lightDir);
        const float vdh = saturate(dot(toCamera, Hn));
        const float ndh = saturate(dot(normal, Hn));
        const float ndl = saturate(dot(normal, lightDir));
        const float ndv = abs(dot(normal, toCamera) + 1e-5);

        #ifdef DIRLIGHT
            float3 diffuseTerm = Diffuse(diff, roughness, ndv, ndl, vdh) * albedoInput.rgb * ndl * lightColor;
        #else
            float3 lightVec = (cLightPosPS.xyz - worldPos) * cLightPosPS.w;
            float lightDist = length(lightVec);
            float3 diffuseTerm = Diffuse(diff, roughness, ndv, ndl, vdh) * albedoInput.rgb * lightColor * rsqrt(Sample2D(LightRampMap, float2(lightDist, 0.0)).r);
        #endif

        // float3 diffuseTerm = ndl * lightColor * diff * albedoInput.rgb;
        float3 fresnelTerm = Fresnel(specColor, vdh);
        float distTerm = Distribution(roughness, ndh);
        float visTerm = GeometricVisibility(roughness, ndv, ndl, vdh);
        
        oColor = float4(diffuseTerm, 1);
        oColor.rgb += distTerm * visTerm * fresnelTerm * lightColor * diff;
    #else
        #ifdef SPECULAR
            float spec = GetSpecular(normal, -worldPos, lightDir, normalInput.a * 255.0);
            oColor = diff * float4(lightColor * (albedoInput.rgb + spec * cLightColor.a * albedoInput.aaa), 0.0);
        #else
            oColor = diff * float4(lightColor * albedoInput.rgb, 0.0);
        #endif
    #endif
}
