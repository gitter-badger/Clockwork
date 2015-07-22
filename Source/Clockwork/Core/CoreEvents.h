

#pragma once

#include "../Core/Object.h"

namespace Clockwork
{

/// Frame begin event.
EVENT(E_BEGINFRAME, BeginFrame)
{
    PARAM(P_FRAMENUMBER, FrameNumber);      // unsigned
    PARAM(P_TIMESTEP, TimeStep);            // float
}

/// Application-wide logic update event.
EVENT(E_UPDATE, Update)
{
    PARAM(P_TIMESTEP, TimeStep);            // float
}

/// Application-wide logic post-update event.
EVENT(E_POSTUPDATE, PostUpdate)
{
    PARAM(P_TIMESTEP, TimeStep);            // float
}

/// Render update event.
EVENT(E_RENDERUPDATE, RenderUpdate)
{
    PARAM(P_TIMESTEP, TimeStep);            // float
}

/// Post-render update event.
EVENT(E_POSTRENDERUPDATE, PostRenderUpdate)
{
    PARAM(P_TIMESTEP, TimeStep);            // float
}

/// Frame end event.
EVENT(E_ENDFRAME, EndFrame)
{
}

}
