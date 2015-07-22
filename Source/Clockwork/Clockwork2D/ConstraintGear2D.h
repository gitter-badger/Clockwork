

#pragma once

#include "../Clockwork2D/Constraint2D.h"

namespace Clockwork
{

/// 2D gear constraint component.
class CLOCKWORK_API ConstraintGear2D : public Constraint2D
{
    OBJECT(ConstraintGear2D);

public:
    /// Construct.
    ConstraintGear2D(Context* context);
    /// Destruct.
    virtual ~ConstraintGear2D();
    /// Register object factory.
    static void RegisterObject(Context* context);

    /// Set owner constraint.
    void SetOwnerConstraint(Constraint2D* constraint);
    /// Set other constraint.
    void SetOtherConstraint(Constraint2D* constraint);
    /// Set ratio.
    void SetRatio(float ratio);

    /// Return owner constraint.
    Constraint2D* GetOwnerConstraint() const { return ownerConstraint_; }

    /// Return other constraint.
    Constraint2D* GetOtherConstraint() const { return otherConstraint_; }

    /// Return ratio.
    float GetRatio() const { return jointDef_.ratio; }

private:
    /// Return joint def.
    virtual b2JointDef* GetJointDef();

    /// Box2D joint def.
    b2GearJointDef jointDef_;
    /// Owner body constraint.
    WeakPtr<Constraint2D> ownerConstraint_;
    /// Other body constraint.
    WeakPtr<Constraint2D> otherConstraint_;
};

}
