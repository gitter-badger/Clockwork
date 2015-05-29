#pragma once

#include "../Core/Object.h"

namespace Clockwork
{

/// Variable timestep scene update.
EVENT(E_SCENEUPDATE, SceneUpdate)
{
    PARAM(P_SCENE, Scene);                  // Scene pointer
    PARAM(P_TIMESTEP, TimeStep);            // float
}

/// Scene subsystem update.
EVENT(E_SCENESUBSYSTEMUPDATE, SceneSubsystemUpdate)
{
    PARAM(P_SCENE, Scene);                  // Scene pointer
    PARAM(P_TIMESTEP, TimeStep);            // float
}

/// Scene transform smoothing update.
EVENT(E_UPDATESMOOTHING, UpdateSmoothing)
{
    PARAM(P_CONSTANT, Constant);            // float
    PARAM(P_SQUAREDSNAPTHRESHOLD, SquaredSnapThreshold);  // float
}

/// Scene drawable update finished. Custom animation (eg. IK) can be done at this point.
EVENT(E_SCENEDRAWABLEUPDATEFINISHED, SceneDrawableUpdateFinished)
{
    PARAM(P_SCENE, Scene);                  // Scene pointer
    PARAM(P_TIMESTEP, TimeStep);            // float
}

/// SmoothedTransform target position changed.
EVENT(E_TARGETPOSITION, TargetPositionChanged)
{
}

/// SmoothedTransform target position changed.
EVENT(E_TARGETROTATION, TargetRotationChanged)
{
}

/// Scene attribute animation update.
EVENT(E_ATTRIBUTEANIMATIONUPDATE, AttributeAnimationUpdate)
{
    PARAM(P_SCENE, Scene);                  // Scene pointer
    PARAM(P_TIMESTEP, TimeStep);            // float
}

/// Attribute animation added to object animation.
EVENT(E_ATTRIBUTEANIMATIONADDED, AttributeAnimationAdded)
{
    PARAM(P_OBJECTANIMATION, ObjectAnimation);               // Object animation pointer
    PARAM(P_ATTRIBUTEANIMATIONNAME, AttributeAnimationName); // String
}

/// Attribute animation removed from object animation.
EVENT(E_ATTRIBUTEANIMATIONREMOVED, AttributeAnimationRemoved)
{
    PARAM(P_OBJECTANIMATION, ObjectAnimation);               // Object animation pointer
    PARAM(P_ATTRIBUTEANIMATIONNAME, AttributeAnimationName); // String
}

/// Variable timestep scene post-update.
EVENT(E_SCENEPOSTUPDATE, ScenePostUpdate)
{
    PARAM(P_SCENE, Scene);                  // Scene pointer
    PARAM(P_TIMESTEP, TimeStep);            // float
}

/// Asynchronous scene loading progress.
EVENT(E_ASYNCLOADPROGRESS, AsyncLoadProgress)
{
    PARAM(P_SCENE, Scene);                  // Scene pointer
    PARAM(P_PROGRESS, Progress);            // float
    PARAM(P_LOADEDNODES, LoadedNodes);      // int
    PARAM(P_TOTALNODES, TotalNodes);        // int
    PARAM(P_LOADEDRESOURCES, LoadedResources); // int
    PARAM(P_TOTALRESOURCES, TotalResources);   // int
};

/// Asynchronous scene loading finished.
EVENT(E_ASYNCLOADFINISHED, AsyncLoadFinished)
{
    PARAM(P_SCENE, Scene);                  // Scene pointer
};

/// A child node has been added to a parent node.
EVENT(E_NODEADDED, NodeAdded)
{
    PARAM(P_SCENE, Scene);                  // Scene pointer
    PARAM(P_PARENT, Parent);                // Node pointer
    PARAM(P_NODE, Node);                    // Node pointer
}

/// A child node is about to be removed from a parent node.
EVENT(E_NODEREMOVED, NodeRemoved)
{
    PARAM(P_SCENE, Scene);                  // Scene pointer
    PARAM(P_PARENT, Parent);                // Node pointer
    PARAM(P_NODE, Node);                    // Node pointer
}

/// A component has been created to a node.
EVENT(E_COMPONENTADDED, ComponentAdded)
{
    PARAM(P_SCENE, Scene);                  // Scene pointer
    PARAM(P_NODE, Node);                    // Node pointer
    PARAM(P_COMPONENT, Component);          // Component pointer
}

/// A component is about to be removed from a node.
EVENT(E_COMPONENTREMOVED, ComponentRemoved)
{
    PARAM(P_SCENE, Scene);                  // Scene pointer
    PARAM(P_NODE, Node);                    // Node pointer
    PARAM(P_COMPONENT, Component);          // Component pointer
}

/// A node's name has changed.
EVENT(E_NODENAMECHANGED, NodeNameChanged)
{
    PARAM(P_SCENE, Scene);                  // Scene pointer
    PARAM(P_NODE, Node);                    // Node pointer
}

/// A node's enabled state has changed.
EVENT(E_NODEENABLEDCHANGED, NodeEnabledChanged)
{
    PARAM(P_SCENE, Scene);                  // Scene pointer
    PARAM(P_NODE, Node);                    // Node pointer
}

/// A component's enabled state has changed.
EVENT(E_COMPONENTENABLEDCHANGED, ComponentEnabledChanged)
{
    PARAM(P_SCENE, Scene);                  // Scene pointer
    PARAM(P_NODE, Node);                    // Node pointer
    PARAM(P_COMPONENT, Component);          // Component pointer
}

/// A serializable's temporary state has changed.
EVENT(E_TEMPORARYCHANGED, TemporaryChanged)
{
    PARAM(P_SERIALIZABLE, Serializable);    // Serializable pointer
}

/// A network attribute update from the server has been intercepted.
EVENT(E_INTERCEPTNETWORKUPDATE, InterceptNetworkUpdate)
{
    PARAM(P_SERIALIZABLE, Serializable);    // Serializable pointer
    PARAM(P_TIMESTAMP, TimeStamp);          // unsigned (0-255)
    PARAM(P_INDEX, Index);                  // unsigned
    PARAM(P_NAME, Name);                    // String
    PARAM(P_VALUE, Value);                  // Variant
}

}
