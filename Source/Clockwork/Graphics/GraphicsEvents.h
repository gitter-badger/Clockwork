

#pragma once

#include "../Core/Object.h"

namespace Clockwork
{

/// New screen mode set.
EVENT(E_SCREENMODE, ScreenMode)
{
    PARAM(P_WIDTH, Width);                  // int
    PARAM(P_HEIGHT, Height);                // int
    PARAM(P_FULLSCREEN, Fullscreen);        // bool
    PARAM(P_RESIZABLE, Resizable);          // bool
    PARAM(P_BORDERLESS, Borderless);        // bool
}

/// Window position changed.
EVENT(E_WINDOWPOS, WindowPos)
{
    PARAM(P_X, X);                          // int
    PARAM(P_Y, Y);                          // int
}

/// Request for queuing autoupdated rendersurfaces.
EVENT(E_RENDERSURFACEUPDATE, RenderSurfaceUpdate)
{
}

/// Frame rendering started.
EVENT(E_BEGINRENDERING, BeginRendering)
{
}

/// Frame rendering ended.
EVENT(E_ENDRENDERING, EndRendering)
{
}

/// Update of a view started.
EVENT(E_BEGINVIEWUPDATE, BeginViewUpdate)
{
    PARAM(P_VIEW, View);                    // View pointer
    PARAM(P_TEXTURE, Texture);              // Texture pointer
    PARAM(P_SURFACE, Surface);              // RenderSurface pointer
    PARAM(P_SCENE, Scene);                  // Scene pointer
    PARAM(P_CAMERA, Camera);                // Camera pointer
}

/// Update of a view ended.
EVENT(E_ENDVIEWUPDATE, EndViewUpdate)
{
    PARAM(P_VIEW, View);                    // View pointer
    PARAM(P_TEXTURE, Texture);              // Texture pointer
    PARAM(P_SURFACE, Surface);              // RenderSurface pointer
    PARAM(P_SCENE, Scene);                  // Scene pointer
    PARAM(P_CAMERA, Camera);                // Camera pointer
}

/// Render of a view started.
EVENT(E_BEGINVIEWRENDER, BeginViewRender)
{
    PARAM(P_VIEW, View);                    // View pointer
    PARAM(P_TEXTURE, Texture);              // Texture pointer
    PARAM(P_SURFACE, Surface);              // RenderSurface pointer
    PARAM(P_SCENE, Scene);                  // Scene pointer
    PARAM(P_CAMERA, Camera);                // Camera pointer
}

/// Render of a view ended.
EVENT(E_ENDVIEWRENDER, EndViewRender)
{
    PARAM(P_VIEW, View);                    // View pointer
    PARAM(P_TEXTURE, Texture);              // Texture pointer
    PARAM(P_SURFACE, Surface);              // RenderSurface pointer
    PARAM(P_SCENE, Scene);                  // Scene pointer
    PARAM(P_CAMERA, Camera);                // Camera pointer
}

/// Graphics context has been lost. Some or all (depending on the API) GPU objects have lost their contents.
EVENT(E_DEVICELOST, DeviceLost)
{
}

/// Graphics context has been recreated after being lost. GPU objects in the "data lost" state can be restored now.
EVENT(E_DEVICERESET, DeviceReset)
{
}

}
