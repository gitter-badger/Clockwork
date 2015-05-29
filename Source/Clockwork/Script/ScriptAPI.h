#pragma once

class asIScriptEngine;

namespace Clockwork
{

class Context;

/// Register the Math library to script.
void RegisterMathAPI(asIScriptEngine* engine);
/// Register the Core library to script.
void RegisterCoreAPI(asIScriptEngine* engine);
/// Register the IO library to script.
void RegisterIOAPI(asIScriptEngine* engine);
/// Register the Resource library to script.
void RegisterResourceAPI(asIScriptEngine* engine);
/// Register the Scene library to script.
void RegisterSceneAPI(asIScriptEngine* engine);
/// Register the Graphics library to script.
void RegisterGraphicsAPI(asIScriptEngine* engine);
/// Register the Input library to script.
void RegisterInputAPI(asIScriptEngine* engine);
/// Register the Audio library to script.
void RegisterAudioAPI(asIScriptEngine* engine);
/// Register the UI library to script.
void RegisterUIAPI(asIScriptEngine* engine);
/// Register the Network library to script.
void RegisterNetworkAPI(asIScriptEngine* engine);
#ifdef CLOCKWORK_PHYSICS
/// Register the Physics library to script.
void RegisterPhysicsAPI(asIScriptEngine* engine);
#endif
#ifdef CLOCKWORK_NAVIGATION
/// Register the Navigation library to script.
void RegisterNavigationAPI(asIScriptEngine* engine);
#endif
/// Register the Clockwork2D library to script.
void RegisterClockwork2DAPI(asIScriptEngine* engine);
/// Register the Script interfaces to script.
void RegisterScriptInterfaceAPI(asIScriptEngine* engine);
/// Register the Script library to script.
void RegisterScriptAPI(asIScriptEngine* engine);
/// Register the Engine library to script.
void RegisterEngineAPI(asIScriptEngine* engine);

}
