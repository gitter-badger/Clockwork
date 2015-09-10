#include "Uniforms.glsl"
#include "Samplers.glsl"
#include "Transform.glsl"
#include "ScreenPos.glsl"
#include "Lighting.glsl"
#include "BRDF.glsl"
#include "DeferredGBuffer.glsl"

#ifdef DIRLIGHT
    varying vec2 vScreenPos;
#else
    varying vec4 vScreenPos;
#endif
varying vec3 vFarRay;
#ifdef ORTHO
    varying vec3 vNearRay;
#endif

#ifdef PBR
    varying vec2 vFragPos;
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
    #endif
    
    #ifdef PBR
        vec3 normal = DecodeGBufferNormal(normalInput.xy, vFarRay);
        float roughness = normalInput.b;
        #ifdef DIRLIGHT
            vec3 specColor = vec3(0.0f, 0.0f, 0.0f);
            vec3 albedoColor = vec3(0.0f, 0.0f, 0.0f);
            DecodeYCoCgElements(vScreenPos.xy, albedoInput, albedoColor, specColor);
            albedoInput.rgb = albedoColor;
            albedoInput.a = 1.0f;
        #else
            vec3 specColor = vec3(0.0f, 0.0f, 0.0f);
            vec3 albedoColor = vec3(0.0f, 0.0f, 0.0f);
            DecodeYCoCgElements(vScreenPos.xy/vScreenPos.w, albedoInput, albedoColor, specColor);
            albedoInput.rgb = albedoColor;
            albedoInput.a = 1.0f;
        #endif
        albedoInput.a = 1.0;
    #else
         vec3 normal = normalize(normalInput.rgb * 2.0 - 1.0);
    #endif
    //vec3 normal = normalize(normalInput.rgb * 2.0 - 1.0);
    vec4 projWorldPos = vec4(worldPos, 1.0);
    vec3 lightColor;
    vec3 lightDir;
    
    float diff = GetDiffuse(normal, worldPos, lightDir);

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
    
    #ifdef PBR          
        vec3 toCamera = normalize(-worldPos);

        vec3 Hn = normalize(toCamera + lightDir);
        float vdh = max(0.0, dot(toCamera, Hn));
        float ndh = max(0.0, dot(normal, Hn));
        float ndl = max(0.0, dot(normal, lightDir));
        float ndv = max(1e-5, dot(normal, toCamera));

        #ifdef DIRLIGHT
            vec3 diffuseTerm = Diffuse(vec3(diff, diff, diff), roughness, ndv, ndl, vdh) * albedoInput.rgb * ndl * lightColor;
        #else
            vec3 lightVec = (cLightPosPS.xyz - worldPos) * cLightPosPS.w;
            float lightDist = length(lightVec);
            vec3 diffuseTerm = Diffuse(vec3(diff, diff, diff), roughness, ndv, ndl, vdh) * albedoInput.rgb * lightColor * inversesqrt(texture2D(sLightRampMap, vec2(lightDist, 0.0)).r);
        #endif

        vec3 fresnelTerm = Fresnel(specColor, vdh);
        float distTerm = Distribution(roughness, ndh);
        float visTerm = GeometricVisibility(roughness, ndv, ndl, vdh);
            
        gl_FragColor = vec4(diffuseTerm, 1);
        #ifdef SPECULAR
        gl_FragColor += vec4(distTerm * visTerm * fresnelTerm * lightColor * diff,0.0);
        #endif
       // gl_FragColor.rgb = LinearFromSRGB(gl_FragColor.rgb);
    #else
        #ifdef SPECULAR
             float spec = GetSpecular(normal, -worldPos, lightDir, normalInput.a * 255.0); 
             gl_FragColor = diff * vec4(lightColor * (albedoInput.rgb + spec * cLightColor.a * albedoInput.aaa), 0.0); 
         #else 
             gl_FragColor = diff * vec4(lightColor * albedoInput.rgb, 0.0); 
         #endif 

    #endif

}
