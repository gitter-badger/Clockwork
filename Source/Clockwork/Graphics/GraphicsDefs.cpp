#include "../Graphics/GraphicsDefs.h"
#include "../Math/StringHash.h"
#include "../Math/Vector3.h"

#include "../DebugNew.h"

namespace Clockwork
{

extern CLOCKWORK_API const StringHash VSP_AMBIENTSTARTCOLOR("AmbientStartColor");
extern CLOCKWORK_API const StringHash VSP_AMBIENTENDCOLOR("AmbientEndColor");
extern CLOCKWORK_API const StringHash VSP_BILLBOARDROT("BillboardRot");
extern CLOCKWORK_API const StringHash VSP_CAMERAPOS("CameraPos");
extern CLOCKWORK_API const StringHash VSP_CAMERAROT("CameraRot");
extern CLOCKWORK_API const StringHash VSP_CLIPPLANE("ClipPlane");
extern CLOCKWORK_API const StringHash VSP_NEARCLIP("NearClip");
extern CLOCKWORK_API const StringHash VSP_FARCLIP("FarClip");
extern CLOCKWORK_API const StringHash VSP_DEPTHMODE("DepthMode");
extern CLOCKWORK_API const StringHash VSP_DELTATIME("DeltaTime");
extern CLOCKWORK_API const StringHash VSP_ELAPSEDTIME("ElapsedTime");
extern CLOCKWORK_API const StringHash VSP_FRUSTUMSIZE("FrustumSize");
extern CLOCKWORK_API const StringHash VSP_GBUFFEROFFSETS("GBufferOffsets");
extern CLOCKWORK_API const StringHash VSP_LIGHTDIR("LightDir");
extern CLOCKWORK_API const StringHash VSP_LIGHTPOS("LightPos");
extern CLOCKWORK_API const StringHash VSP_MODEL("Model");
extern CLOCKWORK_API const StringHash VSP_VIEWPROJ("ViewProj");
extern CLOCKWORK_API const StringHash VSP_UOFFSET("UOffset");
extern CLOCKWORK_API const StringHash VSP_VOFFSET("VOffset");
extern CLOCKWORK_API const StringHash VSP_ZONE("Zone");
extern CLOCKWORK_API const StringHash VSP_LIGHTMATRICES("LightMatrices");
extern CLOCKWORK_API const StringHash VSP_SKINMATRICES("SkinMatrices");
extern CLOCKWORK_API const StringHash VSP_VERTEXLIGHTS("VertexLights");
extern CLOCKWORK_API const StringHash PSP_AMBIENTCOLOR("AmbientColor");
extern CLOCKWORK_API const StringHash PSP_CAMERAPOS("CameraPosPS");
extern CLOCKWORK_API const StringHash PSP_DELTATIME("DeltaTimePS");
extern CLOCKWORK_API const StringHash PSP_DEPTHRECONSTRUCT("DepthReconstruct");
extern CLOCKWORK_API const StringHash PSP_ELAPSEDTIME("ElapsedTimePS");
extern CLOCKWORK_API const StringHash PSP_FOGCOLOR("FogColor");
extern CLOCKWORK_API const StringHash PSP_FOGPARAMS("FogParams");
extern CLOCKWORK_API const StringHash PSP_GBUFFERINVSIZE("GBufferInvSize");
extern CLOCKWORK_API const StringHash PSP_LIGHTCOLOR("LightColor");
extern CLOCKWORK_API const StringHash PSP_LIGHTDIR("LightDirPS");
extern CLOCKWORK_API const StringHash PSP_LIGHTPOS("LightPosPS");
extern CLOCKWORK_API const StringHash PSP_MATDIFFCOLOR("MatDiffColor");
extern CLOCKWORK_API const StringHash PSP_MATEMISSIVECOLOR("MatEmissiveColor");
extern CLOCKWORK_API const StringHash PSP_MATENVMAPECOLOR("MatEnvMapColor");
extern CLOCKWORK_API const StringHash PSP_MATSPECCOLOR("MatSpecColor");
extern CLOCKWORK_API const StringHash PSP_NEARCLIP("NearClipPS");
extern CLOCKWORK_API const StringHash PSP_FARCLIP("FarClipPS");
extern CLOCKWORK_API const StringHash PSP_SHADOWCUBEADJUST("ShadowCubeAdjust");
extern CLOCKWORK_API const StringHash PSP_SHADOWDEPTHFADE("ShadowDepthFade");
extern CLOCKWORK_API const StringHash PSP_SHADOWINTENSITY("ShadowIntensity");
extern CLOCKWORK_API const StringHash PSP_SHADOWMAPINVSIZE("ShadowMapInvSize");
extern CLOCKWORK_API const StringHash PSP_SHADOWSPLITS("ShadowSplits");
extern CLOCKWORK_API const StringHash PSP_LIGHTMATRICES("LightMatricesPS");

extern CLOCKWORK_API const Vector3 DOT_SCALE(1 / 3.0f, 1 / 3.0f, 1 / 3.0f);

}
