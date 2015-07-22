

#pragma once

#include "../Core/Object.h"

namespace Clockwork
{

/// AnimatedModel bone hierarchy created.
EVENT(E_BONEHIERARCHYCREATED, BoneHierarchyCreated)
{
    PARAM(P_NODE, Node);                    // Node pointer
}

/// AnimatedModel animation trigger.
EVENT(E_ANIMATIONTRIGGER, AnimationTrigger)
{
    PARAM(P_NODE, Node);                    // Node pointer
    PARAM(P_NAME, Name);                    // String
    PARAM(P_TIME, Time);                    // Float
    PARAM(P_DATA, Data);                    // User-defined data type
}
/// Terrain geometry created.
EVENT(E_TERRAINCREATED, TerrainCreated)
{
    PARAM(P_NODE, Node);                    // Node pointer
}

}
