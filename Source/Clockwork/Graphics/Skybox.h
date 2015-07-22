

#pragma once

#include "../Graphics/StaticModel.h"

namespace Clockwork
{

/// Static model component with fixed position in relation to the camera.
class CLOCKWORK_API Skybox : public StaticModel
{
    OBJECT(Skybox);

public:
    /// Construct.
    Skybox(Context* context);
    /// Destruct.
    virtual ~Skybox();
    /// Register object factory. StaticModel must be registered first.
    static void RegisterObject(Context* context);

    /// Process octree raycast. May be called from a worker thread.
    virtual void ProcessRayQuery(const RayOctreeQuery& query, PODVector<RayQueryResult>& results);
    /// Calculate distance and prepare batches for rendering. May be called from worker thread(s), possibly re-entrantly.
    virtual void UpdateBatches(const FrameInfo& frame);

protected:
    /// Recalculate the world-space bounding box.
    virtual void OnWorldBoundingBoxUpdate();

    /// Custom world transform per camera.
    HashMap<Camera*, Matrix3x4> customWorldTransforms_;
    /// Last frame counter for knowing when to erase the custom world transforms of previous frame.
    unsigned lastFrame_;
};

}
