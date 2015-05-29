#pragma once

#include "../Clockwork2D/Constraint2D.h"

namespace Clockwork
{

/// 2D distance constraint component.
class CLOCKWORK_API ConstraintDistance2D : public Constraint2D
{
    OBJECT(ConstraintDistance2D);

public:
    /// Construct.
    ConstraintDistance2D(Context* scontext);
    /// Destruct.
    virtual ~ConstraintDistance2D();
    /// Register object factory.
    static void RegisterObject(Context* context);

    /// Set owner body anchor.
    void SetOwnerBodyAnchor(const Vector2& anchor);
    /// Set other body anchor.
    void SetOtherBodyAnchor(const Vector2& anchor);
    /// Set frequency Hz.
    void SetFrequencyHz(float frequencyHz);
    /// Set damping ratio.
    void SetDampingRatio(float dampingRatio);

    /// Return owner body anchor.
    const Vector2& GetOwnerBodyAnchor() const { return ownerBodyAnchor_; }
    /// Return other body anchor.
    const Vector2& GetOtherBodyAnchor() const { return otherBodyAnchor_; }
    /// Return frequency Hz.
    float GetFrequencyHz() const { return jointDef_.frequencyHz; }
    /// Return damping ratio.
    float GetDampingRatio() const { return jointDef_.dampingRatio; }

private:
    /// Return joint def.
    virtual b2JointDef* GetJointDef();

    b2DistanceJointDef jointDef_;
    /// Owner body anchor.
    Vector2 ownerBodyAnchor_;
    /// Other body anchor.
    Vector2 otherBodyAnchor_;
};

}
