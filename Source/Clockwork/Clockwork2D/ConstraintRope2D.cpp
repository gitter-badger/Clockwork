#include "../Clockwork2D/ConstraintRope2D.h"
#include "../Core/Context.h"
#include "../Clockwork2D/PhysicsUtils2D.h"
#include "../Clockwork2D/RigidBody2D.h"

#include "../DebugNew.h"

namespace Clockwork
{

ConstraintRope2D::ConstraintRope2D(Context* context) :
    Constraint2D(context),
    ownerBodyAnchor_(Vector2::ZERO),
    otherBodyAnchor_(Vector2::ZERO)
{

}

ConstraintRope2D::~ConstraintRope2D()
{
}

void ConstraintRope2D::RegisterObject(Context* context)
{
    context->RegisterFactory<ConstraintRope2D>();

    ACCESSOR_ATTRIBUTE("Is Enabled", IsEnabled, SetEnabled, bool, true, AM_DEFAULT);
    ACCESSOR_ATTRIBUTE("Owner Body Anchor", GetOwnerBodyAnchor, SetOwnerBodyAnchor, Vector2, Vector2::ZERO, AM_DEFAULT);
    ACCESSOR_ATTRIBUTE("Other Body Anchor", GetOtherBodyAnchor, SetOtherBodyAnchor, Vector2, Vector2::ZERO, AM_DEFAULT);
    ACCESSOR_ATTRIBUTE("Max Length", GetMaxLength, SetMaxLength, float, 0.0f, AM_DEFAULT);
    COPY_BASE_ATTRIBUTES(Constraint2D);
}

void ConstraintRope2D::SetOwnerBodyAnchor(const Vector2& anchor)
{
    if (anchor == ownerBodyAnchor_)
        return;

    ownerBodyAnchor_ = anchor;

    RecreateJoint();
    MarkNetworkUpdate();
}

void ConstraintRope2D::SetOtherBodyAnchor(const Vector2& anchor)
{
    if (anchor == otherBodyAnchor_)
        return;

    otherBodyAnchor_ = anchor;

    RecreateJoint();
    MarkNetworkUpdate();
}

void ConstraintRope2D::SetMaxLength(float maxLength)
{
    maxLength = Max(0.0f, maxLength);
    if (maxLength == jointDef_.maxLength)
        return;

    jointDef_.maxLength = maxLength;

    RecreateJoint();
    MarkNetworkUpdate();
}

b2JointDef* ConstraintRope2D::GetJointDef()
{
    if (!ownerBody_ || !otherBody_)
        return 0;

    b2Body* bodyA = ownerBody_->GetBody();
    b2Body* bodyB = otherBody_->GetBody();
    if (!bodyA || !bodyB)
        return 0;

    InitializeJointDef(&jointDef_);
    jointDef_.localAnchorA = ToB2Vec2(ownerBodyAnchor_);
    jointDef_.localAnchorB = ToB2Vec2(otherBodyAnchor_);

    return &jointDef_;
}

}
