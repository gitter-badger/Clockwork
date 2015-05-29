#pragma once

#include "../Clockwork2D/Constraint2D.h"

namespace Clockwork
{

/// 2D rope constraint component.
class CLOCKWORK_API ConstraintRope2D : public Constraint2D
{
    OBJECT(ConstraintRope2D);

public:
    /// Construct.
    ConstraintRope2D(Context* scontext);
    /// Destruct.
    virtual ~ConstraintRope2D();
    /// Register object factory.
    static void RegisterObject(Context* context);

    /// Set owner body anchor.
    void SetOwnerBodyAnchor(const Vector2& anchor);
    /// Set other body anchor.
    void SetOtherBodyAnchor(const Vector2& anchor);
    /// Set max length.
    void SetMaxLength(float maxLength);

    /// Return owner body anchor.
    const Vector2& GetOwnerBodyAnchor() const { return ownerBodyAnchor_; }
    /// Return other body anchor.
    const Vector2& GetOtherBodyAnchor() const { return otherBodyAnchor_; }
    /// Return max length.
    float GetMaxLength() const { return jointDef_.maxLength; }

private:
    /// Return joint def.
    virtual b2JointDef* GetJointDef();

    /// Box2D joint def.
    b2RopeJointDef jointDef_;
    /// Owner body anchor.
    Vector2 ownerBodyAnchor_;
    /// Other body anchor.
    Vector2 otherBodyAnchor_;
};

}
