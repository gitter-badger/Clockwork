#pragma once

#include "../Clockwork2D/Constraint2D.h"

namespace Clockwork
{

/// 2D weld constraint component.
class CLOCKWORK_API ConstraintWeld2D : public Constraint2D
{
    OBJECT(ConstraintWeld2D);

public:
    /// Construct.
    ConstraintWeld2D(Context* scontext);
    /// Destruct.
    virtual ~ConstraintWeld2D();
    /// Register object factory.
    static void RegisterObject(Context* context);

    /// Set anchor.
    void SetAnchor(const Vector2& anchor);
    /// Set frequency Hz.
    void SetFrequencyHz(float frequencyHz);
    /// Set damping ratio.
    void SetDampingRatio(float dampingRatio);

    /// Return anchor.
    const Vector2& GetAnchor() const { return anchor_; }
    /// Return frequency Hz.
    float GetFrequencyHz() const { return jointDef_.frequencyHz; }
    /// Return damping ratio.
    float GetDampingRatio() const { return jointDef_.dampingRatio; }

private:
    /// Return joint def.
    virtual b2JointDef* GetJointDef();

    /// Box2D joint def.
    b2WeldJointDef jointDef_;
    /// Anchor.
    Vector2 anchor_;
};

}
