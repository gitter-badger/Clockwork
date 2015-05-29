#include "../Clockwork2D/Constraint2D.h"
#include "../Core/Context.h"
#include "../IO/Log.h"
#include "../Scene/Node.h"
#include "../Clockwork2D/PhysicsUtils2D.h"
#include "../Clockwork2D/RigidBody2D.h"
#include "../Clockwork2D/PhysicsWorld2D.h"
#include "../Scene/Scene.h"

#include "../DebugNew.h"

namespace Clockwork
{

extern const char* CLOCKWORK2D_CATEGORY;

Constraint2D::Constraint2D(Context* context) :
    Component(context),
    joint_(0),
    collideConnected_(false)
{

}

Constraint2D::~Constraint2D()
{
    if (ownerBody_)
        ownerBody_->RemoveConstraint2D(this);

    if (otherBody_)
        otherBody_->RemoveConstraint2D(this);

    ReleaseJoint();
}

void Constraint2D::RegisterObject(Context* context)
{
    ACCESSOR_ATTRIBUTE("Collide Connected", GetCollideConnected, SetCollideConnected, bool, false, AM_DEFAULT);
}

void Constraint2D::OnSetEnabled()
{
    if (IsEnabledEffective())
        CreateJoint();
    else
        ReleaseJoint();
}

void Constraint2D::CreateJoint()
{
    if (joint_)
        return;

    b2JointDef* jointDef = GetJointDef();
    if (jointDef)
    {
        joint_ = physicsWorld_->GetWorld()->CreateJoint(jointDef);
        joint_->SetUserData(this);
    }
}

void Constraint2D::ReleaseJoint()
{
    if (!joint_)
        return;

    if (physicsWorld_)
        physicsWorld_->GetWorld()->DestroyJoint(joint_);

    joint_ = 0;
}

void Constraint2D::SetOtherBody(RigidBody2D* body)
{
    if (body == otherBody_)
        return;

    otherBody_ = body;

    RecreateJoint();
    MarkNetworkUpdate();
}

void Constraint2D::SetCollideConnected(bool collideConnected)
{
    if (collideConnected == collideConnected_)
        return;

    collideConnected_ = collideConnected;

    RecreateJoint();
    MarkNetworkUpdate();
}

void Constraint2D::SetAttachedConstraint(Constraint2D* constraint)
{
    attachedConstraint_ = constraint;
}

void Constraint2D::OnNodeSet(Node* node)
{
    Component::OnNodeSet(node);

    if (node)
    {
        Scene* scene = GetScene();
        physicsWorld_ = scene->GetOrCreateComponent<PhysicsWorld2D>();

        ownerBody_ = node->GetComponent<RigidBody2D>();
        if (!ownerBody_)
        {
            LOGERROR("No right body component in node, can not create constraint");
            return;
        }
    }
}

void Constraint2D::InitializeJointDef(b2JointDef* jointDef)
{
    jointDef->bodyA = ownerBody_->GetBody();
    jointDef->bodyB = otherBody_->GetBody();
    jointDef->collideConnected = collideConnected_;
}

void Constraint2D::RecreateJoint()
{
    if (attachedConstraint_)
        attachedConstraint_->ReleaseJoint();

    ReleaseJoint();
    CreateJoint();

    if (attachedConstraint_)
        attachedConstraint_->CreateJoint();
}

}
