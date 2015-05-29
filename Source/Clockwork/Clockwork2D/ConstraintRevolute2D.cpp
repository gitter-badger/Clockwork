#include "../Clockwork2D/ConstraintRevolute2D.h"
#include "../Core/Context.h"
#include "../Clockwork2D/PhysicsUtils2D.h"
#include "../Clockwork2D/RigidBody2D.h"

#include "../DebugNew.h"

namespace Clockwork
{

ConstraintRevolute2D::ConstraintRevolute2D(Context* context) :
    Constraint2D(context),
    anchor_(Vector2::ZERO)
{
}

ConstraintRevolute2D::~ConstraintRevolute2D()
{
}

void ConstraintRevolute2D::RegisterObject(Context* context)
{
    context->RegisterFactory<ConstraintRevolute2D>();

    ACCESSOR_ATTRIBUTE("Is Enabled", IsEnabled, SetEnabled, bool, true, AM_DEFAULT);
    ACCESSOR_ATTRIBUTE("Anchor", GetAnchor, SetAnchor, Vector2, Vector2::ZERO, AM_DEFAULT);
    ACCESSOR_ATTRIBUTE("Enable Limit", GetEnableLimit, SetEnableLimit, bool, false, AM_DEFAULT);
    ACCESSOR_ATTRIBUTE("Lower Angle", GetLowerAngle, SetLowerAngle, float, 0.0f, AM_DEFAULT);
    ACCESSOR_ATTRIBUTE("Upper Angle", GetUpperAngle, SetUpperAngle, float, 0.0f, AM_DEFAULT);
    ACCESSOR_ATTRIBUTE("Enable Motor", GetEnableMotor, SetEnableMotor, bool, false, AM_DEFAULT);
    ACCESSOR_ATTRIBUTE("Motor Speed", GetMotorSpeed, SetMotorSpeed, float, 0.0f, AM_DEFAULT);
    ACCESSOR_ATTRIBUTE("Max Motor Torque", GetMaxMotorTorque, SetMaxMotorTorque, float, 0.0f, AM_DEFAULT);
    COPY_BASE_ATTRIBUTES(Constraint2D);
}

void ConstraintRevolute2D::SetAnchor(const Vector2& anchor)
{
    if (anchor == anchor_)
        return;

    anchor_ = anchor;

    RecreateJoint();
    MarkNetworkUpdate();
}

void ConstraintRevolute2D::SetEnableLimit(bool enableLimit)
{
    if (enableLimit == jointDef_.enableLimit)
        return;

    jointDef_.enableLimit = enableLimit;

    RecreateJoint();
    MarkNetworkUpdate();
}

void ConstraintRevolute2D::SetLowerAngle(float lowerAngle)
{
    if (lowerAngle == jointDef_.lowerAngle)
        return;

    jointDef_.lowerAngle = lowerAngle;

    RecreateJoint();
    MarkNetworkUpdate();
}

void ConstraintRevolute2D::SetUpperAngle(float upperAngle)
{
    if (upperAngle == jointDef_.upperAngle)
        return;

    jointDef_.upperAngle = upperAngle;

    RecreateJoint();
    MarkNetworkUpdate();
}

void ConstraintRevolute2D::SetEnableMotor(bool enableMotor)
{
    if (enableMotor == jointDef_.enableMotor)
        return;

    jointDef_.enableMotor = enableMotor;

    RecreateJoint();
    MarkNetworkUpdate();
}

void ConstraintRevolute2D::SetMotorSpeed(float motorSpeed)
{
    if (motorSpeed == jointDef_.motorSpeed)
        return;

    jointDef_.motorSpeed = motorSpeed;

    RecreateJoint();
    MarkNetworkUpdate();
}

void ConstraintRevolute2D::SetMaxMotorTorque(float maxMotorTorque)
{
    if (maxMotorTorque == jointDef_.maxMotorTorque)
        return;

    jointDef_.maxMotorTorque = maxMotorTorque;

    RecreateJoint();
    MarkNetworkUpdate();
}

b2JointDef* ConstraintRevolute2D::GetJointDef()
{
    if (!ownerBody_ || !otherBody_)
        return 0;

    b2Body* bodyA = ownerBody_->GetBody();
    b2Body* bodyB = otherBody_->GetBody();
    if (!bodyA || !bodyB)
        return 0;

    jointDef_.Initialize(bodyA, bodyB, ToB2Vec2(anchor_));

    return &jointDef_;
}

}
