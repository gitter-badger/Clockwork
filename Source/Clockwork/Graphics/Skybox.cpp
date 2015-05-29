#include "../Graphics/Batch.h"
#include "../Graphics/Camera.h"
#include "../Core/Context.h"
#include "../Scene/Node.h"
#include "../Graphics/Skybox.h"

#include "../DebugNew.h"

namespace Clockwork
{

extern const char* GEOMETRY_CATEGORY;

Skybox::Skybox(Context* context) :
    StaticModel(context),
    lastFrame_(0)
{
}

Skybox::~Skybox()
{
}

void Skybox::RegisterObject(Context* context)
{
    context->RegisterFactory<Skybox>(GEOMETRY_CATEGORY);

    COPY_BASE_ATTRIBUTES(StaticModel);
}

void Skybox::ProcessRayQuery(const RayOctreeQuery& query, PODVector<RayQueryResult>& results)
{
    // Do not record a raycast result for a skybox, as it would block all other results
}

void Skybox::UpdateBatches(const FrameInfo& frame)
{
    distance_ = 0.0f;

    if (frame.frameNumber_ != lastFrame_)
    {
        customWorldTransforms_.Clear();
        lastFrame_ = frame.frameNumber_;
    }

    // Add camera position to fix the skybox in space. Use effective world transform to take reflection into account
    Matrix3x4 customWorldTransform = node_->GetWorldTransform();
    customWorldTransform.SetTranslation(node_->GetWorldPosition() + frame.camera_->GetEffectiveWorldTransform().Translation());
    HashMap<Camera*, Matrix3x4>::Iterator it = customWorldTransforms_.Insert(MakePair(frame.camera_, customWorldTransform));

    for (unsigned i = 0; i < batches_.Size(); ++i)
    {
        batches_[i].worldTransform_ = &it->second_;
        batches_[i].distance_ = 0.0f;
    }
}

void Skybox::OnWorldBoundingBoxUpdate()
{
    // The skybox is supposed to be visible everywhere, so set a humongous bounding box
    worldBoundingBox_.Define(-M_LARGE_VALUE, M_LARGE_VALUE);
}

}
