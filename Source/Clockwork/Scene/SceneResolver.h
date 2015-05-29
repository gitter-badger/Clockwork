#pragma once

#include "../Container/HashMap.h"
#include "../Container/Ptr.h"

namespace Clockwork
{

class Component;
class Node;

/// Utility class that resolves node & component IDs after a scene or partial scene load.
class CLOCKWORK_API SceneResolver
{
public:
    /// Construct.
    SceneResolver();
    /// Destruct.
    ~SceneResolver();

    /// Reset. Clear all remembered nodes and components.
    void Reset();
    /// Remember a created node.
    void AddNode(unsigned oldID, Node* node);
    /// Remember a created component.
    void AddComponent(unsigned oldID, Component* component);
    /// Resolve component and node ID attributes and reset.
    void Resolve();

private:
    /// Nodes.
    HashMap<unsigned, WeakPtr<Node> > nodes_;
    /// Components.
    HashMap<unsigned, WeakPtr<Component> > components_;
};

}
