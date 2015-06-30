#include "Uniforms.glsl"
#include "Samplers.glsl"
#include "Transform.glsl"
#include "ScreenPos.glsl"
#include "Lighting.glsl"
#include "BRDF.glsl"

#ifdef DIRLIGHT
    varying vec2 vScreenPos;
#else
    varying vec4 vScreenPos;
#endif
varying vec3 vFarRay;
#ifdef ORTHO
    varying vec3 vNearRay;
#endif

void VS()
{
    mat4 modelMatrix = iModelMatrix;
    vec3 worldPos = GetWorldPos(modelMatrix);
    gl_Position = GetClipPos(worldPos);
    #ifdef DIRLIGHT
        vScreenPos = GetScreenPosPreDiv(gl_Position);
        vFarRay = GetFarRay(gl_Position);
        #ifdef ORTHO
            vNearRay = GetNearRay(gl_Position);
        #endif
    #else
        vScreenPos = GetScreenPos(gl_Position);
        vFarRay = GetFarRay(gl_Position) * gl_Position.w;
        #ifdef ORTHO
            vNearRay = GetNearRay(gl_Position) * gl_Position.w;
        #endif
    #endif
}


void PS()
{
    // If rendering a directional light quad, optimize out the w divide
    #ifdef DIRLIGHT
        #ifdef HWDEPTH
            float depth = ReconstructDepth(texture2D(sDepthBuffer, vScreenPos).r);
        #else
            float depth = DecodeDepth(texture2D(sDepthBuffer, vScreenPos).rgb);
        #endif
        #ifdef ORTHO
            vec3 worldPos = mix(vNearRay, vFarRay, depth);
        #else
            vec3 worldPos = vFarRay * depth;
        #endif
        vec4 albedoInput = texture2D(sAlbedoBuffer, vScreenPos);
        vec4 normalInput = texture2D(sNormalBuffer, vScreenPos);
        vec4 propitiesInput = texture2D(sPropitiesMap, vScreenPos);
        vec4 specInput = texture2D(sSpecMap, vScreenPos);
    #else
        #ifdef HWDEPTH
            float depth = ReconstructDepth(texture2DProj(sDepthBuffer, vScreenPos).r);
        #else
            float depth = DecodeDepth(texture2DProj(sDepthBuffer, vScreenPos).rgb);
        #endif
        #ifdef ORTHO
            vec3 worldPos = mix(vNearRay, vFarRay, depth) / vScreenPos.w;
        #else
            vec3 worldPos = vFarRay * depth / vScreenPos.w;
        #endif
        vec4 albedoInput = texture2DProj(sAlbedoBuffer, vScreenPos);
        vec4 normalInput = texture2DProj(sNormalBuffer, vScreenPos);
        #ifdef PBR
          vec4 propitiesInput = texture2DProj(sPropitiesMap, vScreenPos);
        #else
          vec4 propitiesInput = vec4(1,0.7, 1,0.0);
        #endif
        vec4 specInput = texture2DProj(sSpecMap, vScreenPos);
    #endif

    vec3 normal = normalize(normalInput.rgb * 2.0 - 1.0);
    vec4 projWorldPos = vec4(worldPos, 1.0);
    vec3 lightColor;
    vec3 lightDir;
    float lightDist;

    #ifdef DIRLIGHT
        lightDir = cLightDirPS;
        lightDist = 10000000;
    #else
        vec3 lightVec = (cLightPosPS.xyz - worldPos) * cLightPosPS.w;
        lightDist = length(lightVec);
        lightDir = lightVec / lightDist;
    #endif

    vec3 diffColor = albedoInput.rgb * (1.0 * propitiesInput.r);
    vec3 specColor =  mix(0.25 * vec3(propitiesInput.a), albedoInput.rgb, propitiesInput.r);

    vec3 halfVec = normalize(normalize(-worldPos) + lightDir);

    float NdotV = abs(dot(normal, -worldPos))  + 1e-5;
    float NdotL = clamp(dot(normal, lightDir), 0, 1);
    float NdotH = clamp(dot(normal, halfVec), 0, 1);
    float VdotH = clamp(dot(-worldPos, halfVec), 0 ,1);


    vec3 diff = GetPBRDiffuse(diffColor, propitiesInput.g, NdotV, NdotL, VdotH, 0) * texture2D(sLightRampMap, vec2(lightDist, 0.0)).r;

    #ifdef SHADOW
        diff *= GetShadowDeferred(projWorldPos, depth);
    #endif

    #if defined(SPOTLIGHT)
        vec4 spotPos = projWorldPos * cLightMatricesPS[0];
        lightColor = spotPos.w > 0.0 ? texture2DProj(sLightSpotMap, spotPos).rgb * cLightColor.rgb : vec3(0.0);
    #elif defined(CUBEMASK)
        mat3 lightVecRot = mat3(cLightMatricesPS[0][0].xyz, cLightMatricesPS[0][1].xyz, cLightMatricesPS[0][2].xyz);
        lightColor = textureCube(sLightCubeMap, (worldPos - cLightPosPS.xyz) * lightVecRot).rgb * cLightColor.rgb;
    #else
        lightColor = cLightColor.rgb;
    #endif

    #ifdef SPECULAR
        float specDist = GetPBRSpecD(propitiesInput.g, NdotH, 1);
        vec3 specFresnel = GetPBRSpecF(specColor, VdotH);
        float specGeoShadow = GetPBRSpecG(propitiesInput.g,NdotV, NdotL, NdotH);

        vec3 spec = specDist * specFresnel * specGeoShadow;
        vec4 result = vec4(lightColor, 0.0) *  vec4((diff * spec ) * (propitiesInput.b * NdotL * lightColor) ,0.0);
        //vec4 result = vec4(diff * (0) + (specDist * specGeoShadow * 1) * specFresnel,0.0);
        //Diffuse * (LobeEnergy[2] * DiffSpecMask.r) + (D * Vis * DiffSpecMask.g) * F;
        gl_FragColor =  result;
    #else
        gl_FragColor = vec4((diff ) * (propitiesInput.b * NdotL * lightColor) ,0.0);
    #endif
}
