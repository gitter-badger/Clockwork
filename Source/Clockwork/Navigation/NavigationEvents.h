

#pragma once

#include "../Core/Object.h"

namespace Clockwork
{

/// Complete rebuild of navigation mesh.
EVENT(E_NAVIGATION_MESH_REBUILT, NavigationMeshRebuilt)
{
    PARAM(P_NODE, Node); // Node pointer
    PARAM(P_MESH, Mesh); // NavigationMesh pointer
}

/// Partial bounding box rebuild of navigation mesh.
EVENT(E_NAVIGATION_AREA_REBUILT, NavigationAreaRebuilt)
{
    PARAM(P_NODE, Node); // Node pointer
    PARAM(P_MESH, Mesh); // NavigationMesh pointer
    PARAM(P_BOUNDSMIN, BoundsMin); // Vector3
    PARAM(P_BOUNDSMAX, BoundsMax); // Vector3
}

/// Crowd agent has been repositioned.
EVENT(E_CROWD_AGENT_REPOSITION, CrowdAgentReposition)
{
    PARAM(P_NODE, Node); // Node pointer
    PARAM(P_CROWD_AGENT, CrowdAgent); // CrowdAgent pointer
    PARAM(P_POSITION, Position); // Vector3
    PARAM(P_VELOCITY, Velocity); // Vector3
    PARAM(P_ARRIVED, Arrived); // bool
}

/// Crowd agent's internal state has become invalidated.
EVENT(E_CROWD_AGENT_FAILURE, CrowdAgentFailure)
{
    PARAM(P_NODE, Node); // Node pointer
    PARAM(P_CROWD_AGENT, CrowdAgent); // CrowdAgent pointer
    PARAM(P_POSITION, Position); // Vector3
    PARAM(P_VELOCITY, Velocity); // Vector3
    PARAM(P_CROWD_AGENT_STATE, CrowdAgentState); // int
    PARAM(P_CROWD_TARGET_STATE, CrowdTargetState); // int
}

/// Crowd agent's state has been changed.
EVENT(E_CROWD_AGENT_STATE_CHANGED, CrowdAgentStateChanged)
{
    PARAM(P_NODE, Node); // Node pointer
    PARAM(P_CROWD_AGENT, CrowdAgent); // CrowdAgent pointer
    PARAM(P_POSITION, Position); // Vector3
    PARAM(P_VELOCITY, Velocity); // Vector3
    PARAM(P_CROWD_AGENT_STATE, CrowdAgentState); // int
    PARAM(P_CROWD_TARGET_STATE, CrowdTargetState); // int
}

/// Addition of obstacle to dynamic navigation mesh.
EVENT(E_NAVIGATION_OBSTACLE_ADDED, NavigationObstacleAdded)
{
    PARAM(P_NODE, Node); // Node pointer
    PARAM(P_OBSTACLE, Obstacle); // Obstacle pointer
    PARAM(P_POSITION, Position); // Vector3
    PARAM(P_RADIUS, Radius); // float
    PARAM(P_HEIGHT, Height); // float
}

/// Removal of obstacle from dynamic navigation mesh.
EVENT(E_NAVIGATION_OBSTACLE_REMOVED, NavigationObstacleRemoved)
{
    PARAM(P_NODE, Node); // Node pointer
    PARAM(P_OBSTACLE, Obstacle); // Obstacle pointer
    PARAM(P_POSITION, Position); // Vector3
    PARAM(P_RADIUS, Radius); // float
    PARAM(P_HEIGHT, Height); // float
}

}
