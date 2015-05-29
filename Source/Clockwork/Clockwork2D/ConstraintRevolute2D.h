#pragma once

#include "../Clockwork2D/Constraint2D.h"

namespace Clockwork
{

/// 2D revolute constraint component.
class CLOCKWORK_API ConstraintRevolute2D : public Constraint2D
{
    OBJECT(ConstraintRevolute2D);

public:
    /// Construct.
    ConstraintRevolute2D(Context* scontext);
    /// Destruct.
    virtual ~ConstraintRevolute2D();
    /// Register object factory.
    static void RegisterObject(Context* context);

    /// Set anchor.
    void SetAnchor(const Vector2& anchor);
    /// Set enable limit.
    void SetEnableLimit(bool enableLimit);
    /// Set lower angle.
    void SetLowerAngle(float lowerAngle);
    /// Set upper angle.
    void SetUpperAngle(float upperAngle);
    /// Set enable motor.
    void SetEnableMotor(bool enableMotor);
    /// Set motor speed.
    void SetMotorSpeed(float motorSpeed);
    /// Set max motor torque.
    void SetMaxMotorTorque(float maxMotorTorque);

    /// Return anchor.
    const Vector2& GetAnchor() const { return anchor_; }
    /// Return enable limit.
    bool GetEnableLimit() const { return jointDef_.enableLimit; }
    /// Return lower angle.
    float GetLowerAngle() const { return jointDef_.lowerAngle; }
    /// Return upper angle.
    float GetUpperAngle() const { return jointDef_.upperAngle; }
    /// Return enable motor.
    bool GetEnableMotor() const { return jointDef_.enableMotor; }
    /// Return motor speed.
    float GetMotorSpeed() const { return jointDef_.motorSpeed; }
    /// Return max motor torque.
    float GetMaxMotorTorque() const { return jointDef_.maxMotorTorque; }

private:
    /// Return joint def.
    virtual b2JointDef* GetJointDef();

    /// Box2D joint def.
    b2RevoluteJointDef jointDef_;
    /// Anchor.
    Vector2 anchor_;
};

}
