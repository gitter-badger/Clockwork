

#pragma once

#include "../Clockwork2D/Constraint2D.h"

namespace Clockwork
{

/// 2D wheel constraint component.
class CLOCKWORK_API ConstraintWheel2D : public Constraint2D
{
    OBJECT(ConstraintWheel2D);

public:
    /// Construct.
    ConstraintWheel2D(Context* context);
    /// Destruct.
    virtual ~ConstraintWheel2D();
    /// Register object factory.
    static void RegisterObject(Context* context);

    /// Set anchor.
    void SetAnchor(const Vector2& anchor);
    /// Set axis.
    void SetAxis(const Vector2& axis);
    /// Set enable motor.
    void SetEnableMotor(bool enableMotor);
    /// Set max motor torque.
    void SetMaxMotorTorque(float maxMotorTorque);
    /// Set motor speed.
    void SetMotorSpeed(float motorSpeed);
    /// Set frequency Hz.
    void SetFrequencyHz(float frequencyHz);
    /// Set damping ratio.
    void SetDampingRatio(float dampingRatio);

    /// Return anchor.
    const Vector2& GetAnchor() const { return anchor_; }

    /// Return axis.
    const Vector2& GetAxis() const { return axis_; }

    /// Return enable motor.
    bool GetEnableMotor() const { return jointDef_.enableMotor; }

    /// Return maxMotor torque.
    float GetMaxMotorTorque() const { return jointDef_.maxMotorTorque; }

    /// Return motor speed.
    float GetMotorSpeed() const { return jointDef_.motorSpeed; }

    /// Return frequency Hz.
    float GetFrequencyHz() const { return jointDef_.frequencyHz; }

    /// Return damping ratio.
    float GetDampingRatio() const { return jointDef_.dampingRatio; }

private:
    /// Return joint def.
    virtual b2JointDef* GetJointDef();

    /// Box2D joint def.
    b2WheelJointDef jointDef_;
    /// Anchor.
    Vector2 anchor_;
    /// Axis.
    Vector2 axis_;
};

}
