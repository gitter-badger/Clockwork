#pragma once

#include "../Container/Ptr.h"
#include "../Scene/Component.h"

namespace Clockwork
{

class DynamicNavigationMesh;

/// Obstacle for dynamic navigation mesh.
class CLOCKWORK_API Obstacle : public Component
{
    OBJECT(Obstacle)
    friend class DynamicNavigationMesh;

public:
    /// Construct.
    Obstacle(Context*);
    /// Destruct.
    virtual ~Obstacle();

    /// Register Obstacle with engine context.
    static void RegisterObject(Context*);

    /// Update the owning mesh when enabled status has changed.
    virtual void OnSetEnabled();

    /// Get the height of this obstacle.
    float GetHeight() const { return height_; }
    /// Set the height of this obstacle.
    void SetHeight(float);
    /// Get the blocking radius of this obstacle.
    float GetRadius() const { return radius_; }
    /// Set the blocking radius of this obstacle.
    void SetRadius(float);

    /// Get the internal obstacle ID.
    unsigned GetObstacleID() const { return obstacleId_; }

    /// Render debug information.
    virtual void DrawDebugGeometry(DebugRenderer*, bool depthTest);
    /// Simplified rendering of debug information for script usage.
    void DrawDebugGeometry(bool depthTest);

protected:
    /// Handle node being assigned, identify our DynamicNavigationMesh.
    virtual void OnNodeSet(Node* node);

private:
    /// Radius of this obstacle.
    float radius_;
    /// Height of this obstacle, extends 1/2 height below and 1/2 height above the owning node's position.
    float height_;

    /// Id received from tile cache.
    unsigned obstacleId_;
    /// Pointer to the navigation mesh we belong to.
    WeakPtr<DynamicNavigationMesh> ownerMesh_;
};

}
