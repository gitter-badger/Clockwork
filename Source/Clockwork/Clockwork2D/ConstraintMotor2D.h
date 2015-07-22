

#pragma once

#include "../Clockwork2D/Constraint2D.h"

namespace Clockwork
{

/// 2D motor constraint component.
class CLOCKWORK_API ConstraintMotor2D : public Constraint2D
{
    OBJECT(ConstraintMotor2D);

public:
    /// Construct.
    ConstraintMotor2D(Context* context);
    /// Destruct.
    virtual ~ConstraintMotor2D();
    /// Register object factory.
    static void RegisterObject(Context* context);

    /// Set linear offset.
    void SetLinearOffset(const Vector2& linearOffset);
    /// Set angular offset.
    void SetAngularOffset(float angularOffset);
    /// Set max force.
    void SetMaxForce(float maxForce);
    /// Set max torque.
    void SetMaxTorque(float maxTorque);
    /// Set correction factor.
    void SetCorrectionFactor(float correctionFactor);

    /// Return linear offset.
    const Vector2& GetLinearOffset() const { return linearOffset_; }

    /// Return angular offset.
    float GetAngularOffset() const { return jointDef_.angularOffset; }

    /// Return max force.
    float GetMaxForce() const { return jointDef_.maxForce; }

    /// Return max torque.
    float GetMaxTorque() const { return jointDef_.maxTorque; }

    /// Return correction factor.
    float GetCorrectionFactor() const { return jointDef_.correctionFactor; }

private:
    /// Return joint def.
    virtual b2JointDef* GetJointDef();

    /// Box2D joint def.
    b2MotorJointDef jointDef_;
    /// Linear offset.
    Vector2 linearOffset_;
};

}
