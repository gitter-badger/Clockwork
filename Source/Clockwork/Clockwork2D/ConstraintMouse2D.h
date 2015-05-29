#pragma once

#include "../Clockwork2D/Constraint2D.h"

namespace Clockwork
{

/// 2D mouse constraint component.
class CLOCKWORK_API ConstraintMouse2D : public Constraint2D
{
    OBJECT(ConstraintMouse2D);

public:
    /// Construct.
    ConstraintMouse2D(Context* scontext);
    /// Destruct.
    virtual ~ConstraintMouse2D();
    /// Register object factory.
    static void RegisterObject(Context* context);

    /// Set target.
    void SetTarget(const Vector2& target);
    /// Set max force.
    void SetMaxForce(float maxForce);
    /// Set frequency Hz.
    void SetFrequencyHz(float frequencyHz);
    /// Set damping ratio.
    void SetDampingRatio(float dampingRatio);

    /// Return target.
    const Vector2& GetTarget() const { return target_; }
    /// Return max force.
    float GetMaxForce() const { return jointDef_.maxForce; }
    /// Return frequency Hz.
    float GetFrequencyHz() const { return jointDef_.frequencyHz; }
    /// Return damping ratio.
    float GetDampingRatio() const { return jointDef_.dampingRatio; }

private:
    /// Return joint def.
    virtual b2JointDef* GetJointDef();

    /// Box2D joint def.
    b2MouseJointDef jointDef_;
    /// Target.
    Vector2 target_;
    /// Target setted.
    bool targetSetted_;
};

}
