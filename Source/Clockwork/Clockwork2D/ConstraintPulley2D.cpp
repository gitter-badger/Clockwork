#include "../Clockwork2D/ConstraintPulley2D.h"
#include "../Core/Context.h"
#include "../Clockwork2D/PhysicsUtils2D.h"
#include "../Clockwork2D/RigidBody2D.h"

#include "../DebugNew.h"

namespace Clockwork
{

ConstraintPulley2D::ConstraintPulley2D(Context* context) :
    Constraint2D(context),
    ownerBodyGroundAnchor_(-1.0f, 1.0f),
    otherBodyGroundAnchor_(1.0f, 1.0f),
    ownerBodyAnchor_(-1.0f, 0.0f),
    otherBodyAnchor_(1.0f, 0.0f)
{

}

ConstraintPulley2D::~ConstraintPulley2D()
{
}

void ConstraintPulley2D::RegisterObject(Context* context)
{
    context->RegisterFactory<ConstraintPulley2D>();

    ACCESSOR_ATTRIBUTE("Is Enabled", IsEnabled, SetEnabled, bool, true, AM_DEFAULT);
    ACCESSOR_ATTRIBUTE("Owner Body Ground Anchor", GetOwnerBodyGroundAnchor, SetOwnerBodyGroundAnchor, Vector2, Vector2::ZERO, AM_DEFAULT);
    ACCESSOR_ATTRIBUTE("Other Body Ground Anchor", GetOtherBodyGroundAnchor, SetOtherBodyGroundAnchor, Vector2, Vector2::ZERO, AM_DEFAULT);
    ACCESSOR_ATTRIBUTE("Owner Body Anchor", GetOwnerBodyAnchor, SetOwnerBodyAnchor, Vector2, Vector2::ZERO, AM_DEFAULT);
    ACCESSOR_ATTRIBUTE("Other Body Anchor", GetOtherBodyAnchor, SetOtherBodyAnchor, Vector2, Vector2::ZERO, AM_DEFAULT);
    ACCESSOR_ATTRIBUTE("Ratio", GetRatio, SetRatio, float, 0.0f, AM_DEFAULT);
    COPY_BASE_ATTRIBUTES(Constraint2D);
}

void ConstraintPulley2D::SetOwnerBodyGroundAnchor(const Vector2& groundAnchor)
{
    if (groundAnchor == ownerBodyGroundAnchor_)
        return;

    ownerBodyGroundAnchor_ = groundAnchor;

    RecreateJoint();
    MarkNetworkUpdate();
}

void ConstraintPulley2D::SetOtherBodyGroundAnchor(const Vector2& groundAnchor)
{
    if (groundAnchor == otherBodyGroundAnchor_)
        return;

    otherBodyGroundAnchor_ = groundAnchor;

    RecreateJoint();
    MarkNetworkUpdate();
}

void ConstraintPulley2D::SetOwnerBodyAnchor(const Vector2& anchor)
{
    if (anchor == ownerBodyAnchor_)
        return;

    ownerBodyAnchor_ = anchor;

    RecreateJoint();
    MarkNetworkUpdate();
}

void ConstraintPulley2D::SetOtherBodyAnchor(const Vector2& anchor)
{
    if (anchor == otherBodyAnchor_)
        return;

    otherBodyAnchor_ = anchor;

    RecreateJoint();
    MarkNetworkUpdate();
}

void ConstraintPulley2D::SetRatio(float ratio)
{
    if (ratio == jointDef_.ratio)
        return;

    jointDef_.ratio = ratio;

    RecreateJoint();
    MarkNetworkUpdate();
}

b2JointDef* ConstraintPulley2D::GetJointDef()
{
    if (!ownerBody_ || !otherBody_)
        return 0;

    b2Body* bodyA = ownerBody_->GetBody();
    b2Body* bodyB = otherBody_->GetBody();
    if (!bodyA || !bodyB)
        return 0;

    jointDef_.Initialize(bodyA, bodyB, ToB2Vec2(ownerBodyGroundAnchor_), ToB2Vec2(otherBodyGroundAnchor_), ToB2Vec2(ownerBodyAnchor_), ToB2Vec2(otherBodyAnchor_), jointDef_.ratio);

    return &jointDef_;
}

}
