#include "../Navigation/Obstacle.h"

#include "../Core/Context.h"
#include "../Graphics/DebugRenderer.h"
#include "../Navigation/DynamicNavigationMesh.h"
#include "../IO/Log.h"
#include "../Navigation/NavigationEvents.h"
#include "../Scene/Scene.h"

#include "../DebugNew.h"

namespace Clockwork
{

extern const char* NAVIGATION_CATEGORY;

Obstacle::Obstacle(Context* context) :
    Component(context),
    height_(5.0f),
    radius_(5.0f),
    obstacleId_(0)
{
}

Obstacle::~Obstacle()
{
    if (obstacleId_ > 0 && ownerMesh_)
        ownerMesh_->RemoveObstacle(this);
}

void Obstacle::RegisterObject(Context* context)
{
    context->RegisterFactory<Obstacle>(NAVIGATION_CATEGORY);
    COPY_BASE_ATTRIBUTES(Component);
    ACCESSOR_ATTRIBUTE("Radius", GetRadius, SetRadius, float, 5.0f, AM_DEFAULT);
    ACCESSOR_ATTRIBUTE("Height", GetHeight, SetHeight, float, 5.0f, AM_DEFAULT);
}

void Obstacle::OnSetEnabled()
{
    if (ownerMesh_)
    {
        if (IsEnabledEffective())
            ownerMesh_->AddObstacle(this);
        else
            ownerMesh_->RemoveObstacle(this);
    }
}

void Obstacle::SetHeight(float newHeight)
{
    height_ = newHeight;
    if (ownerMesh_)
        ownerMesh_->ObstacleChanged(this);
    MarkNetworkUpdate();
}

void Obstacle::SetRadius(float newRadius)
{
    radius_ = newRadius;
    if (ownerMesh_)
        ownerMesh_->ObstacleChanged(this);
    MarkNetworkUpdate();
}

void Obstacle::OnNodeSet(Node* node)
{
    if (node)
    {
        if (GetScene() == node)
        {
            LOGWARNING(GetTypeName() + " should not be created to the root scene node");
            return;
        }
        if (!ownerMesh_)
            ownerMesh_ = GetScene()->GetComponent<DynamicNavigationMesh>();
        if (ownerMesh_)
            ownerMesh_->AddObstacle(this);
    }
    else
    {
        if (obstacleId_ > 0 && ownerMesh_)
            ownerMesh_->RemoveObstacle(this);
    }
}

void Obstacle::DrawDebugGeometry(DebugRenderer* debug, bool depthTest)
{
    if (debug && IsEnabledEffective())
        debug->AddCylinder(node_->GetWorldPosition(), radius_, height_, Color(0.0f, 1.0f, 1.0f), depthTest);
}

void Obstacle::DrawDebugGeometry(bool depthTest)
{
    Scene* scene = GetScene();
    if (scene)
        DrawDebugGeometry(scene->GetComponent<DebugRenderer>(), depthTest);
}

}
