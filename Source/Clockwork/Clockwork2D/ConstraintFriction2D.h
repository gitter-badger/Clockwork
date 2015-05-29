#pragma once

#include "../Clockwork2D/Constraint2D.h"

namespace Clockwork
{

/// 2D friction constraint component.
class CLOCKWORK_API ConstraintFriction2D : public Constraint2D
{
    OBJECT(ConstraintFriction2D);

public:
    /// Construct.
    ConstraintFriction2D(Context* scontext);
    /// Destruct.
    virtual ~ConstraintFriction2D();
    /// Register object factory.
    static void RegisterObject(Context* context);

    /// Set anchor.
    void SetAnchor(const Vector2& anchor);
    /// Set max force.
    void SetMaxForce(float maxForce);
    /// Set max torque.
    void SetMaxTorque(float maxTorque);

    /// Return anchor.
    const Vector2& GetAnchor() const { return anchor_; }
    /// Set max force.
    float GetMaxForce() const { return jointDef_.maxForce; }
    /// Set max torque.
    float GetMaxTorque() const { return jointDef_.maxTorque; }

private:
    /// Return joint def.
    virtual b2JointDef* GetJointDef();

    /// Box2D joint def.
    b2FrictionJointDef jointDef_;
    /// Anchor.
    Vector2 anchor_;
};

}
