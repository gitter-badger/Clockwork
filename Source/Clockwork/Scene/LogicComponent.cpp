#include "../IO/Log.h"
#include "../Scene/LogicComponent.h"
#ifdef CLOCKWORK_PHYSICS
#include "../Physics/PhysicsEvents.h"
#include "../Physics/PhysicsWorld.h"
#endif
#include "../Scene/Scene.h"
#include "../Scene/SceneEvents.h"

namespace Clockwork
{

LogicComponent::LogicComponent(Context* context) :
    Component(context),
    updateEventMask_(USE_UPDATE | USE_POSTUPDATE | USE_FIXEDUPDATE | USE_FIXEDPOSTUPDATE),
    currentEventMask_(0),
    delayedStartCalled_(false)
{
}

LogicComponent::~LogicComponent()
{
}

void LogicComponent::OnSetEnabled()
{
    UpdateEventSubscription();
}

void LogicComponent::Update(float timeStep)
{
}

void LogicComponent::PostUpdate(float timeStep)
{
}

void LogicComponent::FixedUpdate(float timeStep)
{
}

void LogicComponent::FixedPostUpdate(float timeStep)
{
}

void LogicComponent::SetUpdateEventMask(unsigned char mask)
{
    if (updateEventMask_ != mask)
    {
        updateEventMask_ = mask;
        UpdateEventSubscription();
    }
}

void LogicComponent::OnNodeSet(Node* node)
{
    if (node)
    {
        // We have been attached to a node. Set initial update event subscription state
        UpdateEventSubscription();
        // Then execute the user-defined start function
        Start();
    }
    else
    {
        // We are being detached from a node: execute user-defined stop function and prepare for destruction
        Stop();
    }
}

void LogicComponent::UpdateEventSubscription()
{
    // If scene node is not assigned yet, no need to update subscription
    if (!node_)
        return;

    Scene* scene = GetScene();
    if (!scene)
    {
        LOGWARNING("Node is detached from scene, can not subscribe to update events");
        return;
    }

    bool enabled = IsEnabledEffective();

    bool needUpdate = enabled && ((updateEventMask_ & USE_UPDATE) || !delayedStartCalled_);
    if (needUpdate && !(currentEventMask_ & USE_UPDATE))
    {
        SubscribeToEvent(scene, E_SCENEUPDATE, HANDLER(LogicComponent, HandleSceneUpdate));
        currentEventMask_ |= USE_UPDATE;
    }
    else if (!needUpdate && (currentEventMask_ & USE_UPDATE))
    {
        UnsubscribeFromEvent(scene, E_SCENEUPDATE);
        currentEventMask_ &= ~USE_UPDATE;
    }

    bool needPostUpdate = enabled && (updateEventMask_ & USE_POSTUPDATE);
    if (needPostUpdate && !(currentEventMask_ & USE_POSTUPDATE))
    {
        SubscribeToEvent(scene, E_SCENEPOSTUPDATE, HANDLER(LogicComponent, HandleScenePostUpdate));
        currentEventMask_ |= USE_POSTUPDATE;
    }
    else if (!needUpdate && (currentEventMask_ & USE_POSTUPDATE))
    {
        UnsubscribeFromEvent(scene, E_SCENEPOSTUPDATE);
        currentEventMask_ &= ~USE_POSTUPDATE;
    }

#ifdef CLOCKWORK_PHYSICS
    PhysicsWorld* world = scene->GetComponent<PhysicsWorld>();
    if (!world)
        return;

    bool needFixedUpdate = enabled && (updateEventMask_ & USE_FIXEDUPDATE);
    if (needFixedUpdate && !(currentEventMask_ & USE_FIXEDUPDATE))
    {
        SubscribeToEvent(world, E_PHYSICSPRESTEP, HANDLER(LogicComponent, HandlePhysicsPreStep));
        currentEventMask_ |= USE_FIXEDUPDATE;
    }
    else if (!needFixedUpdate && (currentEventMask_ & USE_FIXEDUPDATE))
    {
        UnsubscribeFromEvent(world, E_PHYSICSPRESTEP);
        currentEventMask_ &= ~USE_FIXEDUPDATE;
    }

    bool needFixedPostUpdate = enabled && (updateEventMask_ & USE_FIXEDPOSTUPDATE);
    if (needFixedPostUpdate && !(currentEventMask_ & USE_FIXEDPOSTUPDATE))
    {
        SubscribeToEvent(world, E_PHYSICSPOSTSTEP, HANDLER(LogicComponent, HandlePhysicsPostStep));
        currentEventMask_ |= USE_FIXEDPOSTUPDATE;
    }
    else if (!needFixedPostUpdate && (currentEventMask_ & USE_FIXEDPOSTUPDATE))
    {
        UnsubscribeFromEvent(world, E_PHYSICSPOSTSTEP);
        currentEventMask_ &= ~USE_FIXEDPOSTUPDATE;
    }
#endif
}

void LogicComponent::HandleSceneUpdate(StringHash eventType, VariantMap& eventData)
{
    using namespace SceneUpdate;

    // Execute user-defined delayed start function before first update
    if (!delayedStartCalled_)
    {
        DelayedStart();
        delayedStartCalled_ = true;

        // If did not need actual update events, unsubscribe now
        if (!(updateEventMask_ & USE_UPDATE))
        {
            UnsubscribeFromEvent(GetScene(), E_SCENEUPDATE);
            currentEventMask_ &= ~USE_UPDATE;
            return;
        }
    }

    // Then execute user-defined update function
    Update(eventData[P_TIMESTEP].GetFloat());
}

void LogicComponent::HandleScenePostUpdate(StringHash eventType, VariantMap& eventData)
{
    using namespace ScenePostUpdate;

    // Execute user-defined post-update function
    PostUpdate(eventData[P_TIMESTEP].GetFloat());
}

#ifdef CLOCKWORK_PHYSICS
void LogicComponent::HandlePhysicsPreStep(StringHash eventType, VariantMap& eventData)
{
    using namespace PhysicsPreStep;

    // Execute user-defined fixed update function
    FixedUpdate(eventData[P_TIMESTEP].GetFloat());
}

void LogicComponent::HandlePhysicsPostStep(StringHash eventType, VariantMap& eventData)
{
    using namespace PhysicsPostStep;

    // Execute user-defined fixed post-update function
    FixedPostUpdate(eventData[P_TIMESTEP].GetFloat());
}
#endif

}