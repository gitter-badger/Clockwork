#include "../Clockwork2D/ConstraintGear2D.h"
#include "../Core/Context.h"
#include "../Clockwork2D/PhysicsUtils2D.h"
#include "../Clockwork2D/RigidBody2D.h"

#include "../DebugNew.h"

namespace Clockwork
{

ConstraintGear2D::ConstraintGear2D(Context* context) :
    Constraint2D(context)
{
}

ConstraintGear2D::~ConstraintGear2D()
{
}

void ConstraintGear2D::RegisterObject(Context* context)
{
    context->RegisterFactory<ConstraintGear2D>();

    ACCESSOR_ATTRIBUTE("Is Enabled", IsEnabled, SetEnabled, bool, true, AM_DEFAULT);
    ACCESSOR_ATTRIBUTE("Ratio", GetRatio, SetRatio, float, 0.0f, AM_DEFAULT);
    COPY_BASE_ATTRIBUTES(Constraint2D);
}

void ConstraintGear2D::SetOwnerConstraint(Constraint2D* constraint)
{
    if (constraint == ownerConstraint_)
        return;

    if (ownerConstraint_)
        ownerConstraint_->SetAttachedConstraint(0);

    ownerConstraint_ = constraint;

    if (ownerConstraint_)
        ownerConstraint_->SetAttachedConstraint(this);

    RecreateJoint();
    MarkNetworkUpdate();
}

void ConstraintGear2D::SetOtherConstraint(Constraint2D* constraint)
{
    WeakPtr<Constraint2D> constraintPtr(constraint);
    if (constraintPtr == otherConstraint_)
        return;

    if (otherConstraint_)
        otherConstraint_->SetAttachedConstraint(0);

    otherConstraint_ = constraintPtr;

    if (otherConstraint_)
        otherConstraint_->SetAttachedConstraint(this);

    RecreateJoint();
    MarkNetworkUpdate();
}

void ConstraintGear2D::SetRatio(float ratio)
{
    if (ratio == jointDef_.ratio)
        return;

    jointDef_.ratio = ratio;

    RecreateJoint();
    MarkNetworkUpdate();
}

b2JointDef* ConstraintGear2D::GetJointDef()
{
    if (!ownerBody_ || !otherBody_)
        return 0;

    b2Body* bodyA = ownerBody_->GetBody();
    b2Body* bodyB = otherBody_->GetBody();
    if (!bodyA || !bodyB)
        return 0;

    if (!ownerConstraint_ || !otherConstraint_)
        return 0;

    b2Joint* jointA = ownerConstraint_->GetJoint();
    b2Joint* jointB = otherConstraint_->GetJoint();
    if (!jointA || !jointB)
        return 0;

    InitializeJointDef(&jointDef_);
    jointDef_.joint1 = jointA;
    jointDef_.joint2 = jointB;

    return &jointDef_;
}

}
