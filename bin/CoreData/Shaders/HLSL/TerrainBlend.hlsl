#include "Uniforms.hlsl"
#include "Samplers.hlsl"
#include "Transform.hlsl"
#include "ScreenPos.hlsl"
#include "Lighting.hlsl"
#include "Fog.hlsl"
#include "BRDF.hlsl"
#include "DeferredGBuffer.hlsl"

#ifndef D3D11

// D3D9 uniforms and samplers
#ifdef COMPILEVS
uniform float2 cDetailTiling;
#else
sampler2D sWeightMap0 : register(s0);
sampler2D sDetailMap1 : register(s1);
sampler2D sDetailMap2 : register(s2);
sampler2D sDetailMap3 : register(s3);
#endif

#else

// D3D11 constant buffers and samplers
#ifdef COMPILEVS
cbuffer CustomVS : register(b6)
{
    float2 cDetailTiling;
}
#else
Texture2D tWeightMap0 : register(t0);
Texture2D tDetailMap1 : register(t1);
Texture2D tDetailMap2 : register(t2);
Texture2D tDetailMap3 : register(t3);
SamplerState sWeightMap0 : register(s0);
SamplerState sDetailMap1 : register(s1);
SamplerState sDetailMap2 : register(s2);
SamplerState sDetailMap3 : register(s3);
#endif

#endif

void VS(float4 iPos : POSITION,
    float3 iNormal : NORMAL,
    float2 iTexCoord : TEXCOORD0,
    #ifdef SKINNED
        float4 iBlendWeights : BLENDWEIGHT,
        int4 iBlendIndices : BLENDINDICES,
    #endif
    #ifdef INSTANCED
        float4x3 iModelInstance : TEXCOORD2,
    #endif
    #ifdef BILLBOARD
        float2 iSize : TEXCOORD1,
    #endif
    out float2 oTexCoord : TEXCOORD0,
    out float3 oNormal : TEXCOORD1,
    out float4 oWorldPos : TEXCOORD2,
    out float2 oDetailTexCoord : TEXCOORD3,
    #ifdef PERPIXEL
        #ifdef SHADOW
            out float4 oShadowPos[NUMCASCADES] : TEXCOORD4,
        #endif
        #ifdef SPOTLIGHT
            out float4 oSpotPos : TEXCOORD5,
        #endif
        #ifdef POINTLIGHT
            out float3 oCubeMaskVec : TEXCOORD5,
        #endif
    #else
        out float3 oVertexLight : TEXCOORD4,
        out float4 oScreenPos : TEXCOORD5,
    #endif
    #if defined(D3D11) && defined(CLIPPLANE)
        out float oClip : SV_CLIPDISTANCE0,
    #endif
    out float4 oPos : OUTPOSITION)
{
    float4x3 modelMatrix = iModelMatrix;
    float3 worldPos = GetWorldPos(modelMatrix);
    oPos = GetClipPos(worldPos);
    oNormal = GetWorldNormal(modelMatrix);
    oWorldPos = float4(worldPos, GetDepth(oPos));
    oTexCoord = GetTexCoord(iTexCoord);
    oDetailTexCoord = cDetailTiling * oTexCoord;

    #if defined(D3D11) && defined(CLIPPLANE)
        oClip = dot(oPos, cClipPlane);
    #endif

    #ifdef PERPIXEL
        // Per-pixel forward lighting
        float4 projWorldPos = float4(worldPos.xyz, 1.0);

        #ifdef SHADOW
            // Shadow projection: transform from world space to shadow space
            GetShadowPos(projWorldPos, oShadowPos);
        #endif

        #ifdef SPOTLIGHT
            // Spotlight projection: transform from world space to projector texture coordinates
            oSpotPos = mul(projWorldPos, cLightMatrices[0]);
        #endif

        #ifdef POINTLIGHT
            oCubeMaskVec = mul(worldPos - cLightPos.xyz, (float3x3)cLightMatrices[0]);
        #endif
    #else
        // Ambient & per-vertex lighting
        oVertexLight = GetAmbient(GetZonePos(worldPos));

        #ifdef NUMVERTEXLIGHTS
            for (int i = 0; i < NUMVERTEXLIGHTS; ++i)
                oVertexLight += GetVertexLight(i, worldPos, oNormal) * cVertexLights[i * 3].rgb;
        #endif
        
        oScreenPos = GetScreenPos(oPos);
    #endif
}

void PS(float2 iTexCoord : TEXCOORD0,
    float3 iNormal : TEXCOORD1,
    float4 iWorldPos : TEXCOORD2,
    float2 iDetailTexCoord : TEXCOORD3,
    #ifdef PERPIXEL
        #ifdef SHADOW
            float4 iShadowPos[NUMCASCADES] : TEXCOORD4,
        #endif
        #ifdef SPOTLIGHT
            float4 iSpotPos : TEXCOORD5,
        #endif
        #ifdef CUBEMASK
            float3 iCubeMaskVec : TEXCOORD5,
        #endif
    #else
        float3 iVertexLight : TEXCOORD4,
        float4 iScreenPos : TEXCOORD5,
    #endif
    #if defined(D3D11) && defined(CLIPPLANE)
        float iClip : SV_CLIPDISTANCE0,
    #endif
    #ifdef PREPASS
        out float4 oDepth : OUTCOLOR1,
    #endif
    #ifdef DEFERRED
        out float4 oAlbedo : OUTCOLOR1,
        out float4 oNormal : OUTCOLOR2,
        out float4 oDepth : OUTCOLOR3,
        #ifdef PBR
            #ifndef D3D11
                float2 iFragPos : VPOS,
            #else
                float4 iFragPos : SV_Position,
            #endif
        #endif
    #endif
    out float4 oColor : OUTCOLOR0)
{
    // Get material diffuse albedo
    float3 weights = Sample2D(WeightMap0, iTexCoord).rgb;
    float sumWeights = weights.r + weights.g + weights.b;
    weights /= sumWeights;
    float4 diffColor = cMatDiffColor * (
        weights.r * Sample2D(DetailMap1, iDetailTexCoord) +
        weights.g * Sample2D(DetailMap2, iDetailTexCoord) +
        weights.b * Sample2D(DetailMap3, iDetailTexCoord)
    );

    #ifdef PBR
        #ifdef SPECMAP
            #ifdef GLOSSY_SPECULAR
                float4 specSample = Sample2D(SpecGlossMap, iTexCoord.xy);
                float3 specColor = specSample.rgb;
                float roughness = max(0.04, 1.0 - specSample.a);
                roughness = roughness * roughness;
                specColor *= cMatSpecColor.rgb; // mix in externally defined color
            #else
                float4 roughMetalSrc = Sample2D(RoughMetalFresnel, iTexCoord.xy);
                const float roughness = max(0.04, roughMetalSrc.r);
                const float metalness = roughMetalSrc.g;
    
                float3 specColor = max(diffColor.rgb * metalness, float3(0.08, 0.08, 0.08));
                specColor *= cMatSpecColor.rgb;
                diffColor.rgb = diffColor.rgb - diffColor.rgb * metalness; // Modulate down the diffuse
            #endif
         
        #else
            const float roughness = 0.7;
            const float metalness = 0.0;
    
            float3 specColor = max(diffColor.rgb * metalness, float3(0.08, 0.08, 0.08));
            diffColor.rgb = diffColor.rgb - diffColor.rgb * metalness; // Modulate down the diffuse
           
        #endif
    // Get material specular albedo
    #elif defined(SPECMAP)
        float3 specColor = cMatSpecColor.rgb * Sample2D(SpecMap, iTexCoord.xy).rgb;
    #else
        float3 specColor = cMatSpecColor.rgb;
    #endif

    // Get normal
    float3 normal = normalize(iNormal);

    // Get fog factor
    #ifdef HEIGHTFOG
        float fogFactor = GetHeightFogFactor(iWorldPos.w, iWorldPos.y);
    #else
        float fogFactor = GetFogFactor(iWorldPos.w);
    #endif

    #if defined(PERPIXEL)
        // Per-pixel forward lighting
        float3 lightDir;
        float3 lightColor;
        float3 finalColor;
        
        float diff = GetDiffuse(normal, iWorldPos.xyz, lightDir);

        #ifdef SHADOW
            diff *= GetShadow(iShadowPos, iWorldPos.w);
        #endif
    
        #if defined(SPOTLIGHT)
            lightColor = iSpotPos.w > 0.0 ? Sample2DProj(LightSpotMap, iSpotPos).rgb * cLightColor.rgb : 0.0;
        #elif defined(CUBEMASK)
            lightColor = SampleCube(LightCubeMap, iCubeMaskVec).rgb * cLightColor.rgb;
        #else
            lightColor = cLightColor.rgb;
        #endif
    
        #ifdef PBR
            float3 toCamera = normalize(-worldPos);
                
            const float3 Hn = normalize(toCamera + lightDir);
            const float vdh = max(0.0, dot(toCamera, Hn));
            const float ndh = max(0.0, dot(normal, Hn));
            const float ndl = max(0.0, dot(normal, lightDir));
            const float ndv = max(1e-5, dot(normal, toCamera));   
            
            #ifdef DIRLIGHT
            float3 diffuseTerm = Diffuse(diff, roughness, ndv, ndl, vdh) * diffColor.rgb * ndl * lightColor;
            #else
                float3 lightVec = (cLightPosPS.xyz - worldPos) * cLightPosPS.w;
                float lightDist = length(lightVec);
                float3 diffuseTerm = Diffuse(diff, roughness, ndv, ndl, vdh) * diffColor.rgb * lightColor * Sample2D(LightRampMap, float2(lightDist, 0.0)).r)
            #endif

            // float3 diffuseTerm = ndl * lightColor * diff * albedoInput.rgb;
            float3 fresnelTerm = Fresnel(specColor, vdh);
            float distTerm = Distribution(roughness, ndh);
            float visTerm = GeometricVisibility(roughness, ndv, ndl, vdh);
                
            finalColor = float4(diffuseTerm, 1);
            finalColor += distTerm * visTerm * fresnelTerm * lightColor * diff;
            finalColor.rgb = LinearFromSRGB(finalColor.rgb);
        #else
            #ifdef SPECULAR
                float spec = GetSpecular(normal, cCameraPosPS - iWorldPos.xyz, lightDir, cMatSpecColor.a);
                finalColor = diff * lightColor * (diffColor.rgb + spec * specColor * cLightColor.a);
            #else
                finalColor = diff * lightColor * diffColor.rgb;
            #endif
        #endif

        #ifdef AMBIENT
            finalColor += cAmbientColor * diffColor.rgb;
            finalColor += cMatEmissiveColor;
            oColor = float4(GetFog(finalColor, fogFactor), diffColor.a);
        #else
            oColor = float4(GetLitFog(finalColor, fogFactor), diffColor.a);
        #endif
    #elif defined(PREPASS)
        // Fill light pre-pass G-Buffer
        float specPower = cMatSpecColor.a / 255.0;

        oColor = float4(normal * 0.5 + 0.5, specPower);
        oDepth = iWorldPos.w;
    #elif defined(DEFERRED)
        // Fill deferred G-buffer
        float specIntensity = specColor.g;
        float specPower = cMatSpecColor.a / 255.0;

        float3 finalColor = iVertexLight * diffColor.rgb;
        
         #ifdef AO
            #ifdef IBL
                const float aoFactor = Sample2D(EmissiveMap, iTexCoord).r;
            #else
                // If using AO, the vertex light ambient is black, calculate occluded ambient here
                finalColor += Sample2D(EmissiveMap, iTexCoord2).rgb * cAmbientColor * diffColor.rgb;
            #endif
        #endif
        
        #if defined(PBR) || defined(IBL)
            const float3 toCamera = normalize(cCameraPosPS - iWorldPos.xyz);
        #endif
        
        #ifdef IBL
            float3 reflection = normalize(reflect(toCamera, normal));
            float3 cubeColor = iVertexLight.rgb;
            float3 iblColor = ImageBasedLighting(reflection, normal, toCamera, specColor, roughness, cubeColor);
            
           // finalColor = LinearFromSRGB(iVertexLight * ((cubeColor * diffColor ) + iblColor * diffColor * (1 - roughness )));

        #endif

        #if defined(PBR)
            WriteGBuffer(oAlbedo, oNormal, oDepth, toCamera, iFragPos.xy, diffColor, specColor, normal, iWorldPos.w, roughness);
            oColor = float4(GetFog(finalColor, fogFactor), 1.0);
        #else
            oColor = float4(GetFog(finalColor, fogFactor), 1.0);
            oAlbedo = fogFactor * float4(diffColor.rgb, specIntensity);
            oNormal = float4(normal * 0.5 + 0.5, specPower);
            oDepth = iWorldPos.w;
        #endif
    #else
        // Ambient & per-vertex lighting
        float3 finalColor = iVertexLight * diffColor.rgb;
        
         #ifdef AO
            #ifdef IBL
                const float aoFactor = Sample2D(EmissiveMap, iTexCoord).r;
            #else
                // If using AO, the vertex light ambient is black, calculate occluded ambient here
                finalColor += Sample2D(EmissiveMap, iTexCoord2).rgb * cAmbientColor * diffColor.rgb;
            #endif
        #endif
        
        #if defined(PBR) || defined(IBL)
            const float3 toCamera = normalize(cCameraPosPS - iWorldPos.xyz);
        #endif
        
        #ifdef IBL
            float3 reflection = normalize(reflect(toCamera, normal));
            float3 cubeColor = iVertexLight.rgb;
            float3 iblColor = ImageBasedLighting(reflection, normal, toCamera, specColor, roughness, cubeColor);
            #ifdef AO
                finalColor = LinearFromSRGB(iVertexLight * ((cubeColor * diffColor * aoFactor) + iblColor * diffColor.rgb * (1 - roughness ) * aoFactor));
            #else
                finalColor = LinearFromSRGB(iVertexLight * ((cubeColor * diffColor ) + iblColor * diffColor * (1 - roughness )));
            #endif
        #endif

        #ifdef MATERIAL
            // Add light pre-pass accumulation result
            // Lights are accumulated at half intensity. Bring back to full intensity now
            float4 lightInput = 2.0 * Sample2DProj(LightBuffer, iScreenPos);
            float3 lightSpecColor = lightInput.a * (lightInput.rgb / GetIntensity(lightInput.rgb));

            finalColor += lightInput.rgb * diffColor.rgb + lightSpecColor * specColor;
        #endif

        oColor = float4(GetFog(finalColor, fogFactor), diffColor.a);
    #endif
}
