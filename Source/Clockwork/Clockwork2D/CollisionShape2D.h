#pragma once

#include "../Scene/Component.h"
#include <Box2D/Box2D.h>

namespace Clockwork
{

class RigidBody2D;

/// 2D collision shape component.
class CLOCKWORK_API CollisionShape2D : public Component
{
    OBJECT(CollisionShape2D);

public:
    /// Construct.
    CollisionShape2D(Context* scontext);
    /// Destruct.
    virtual ~CollisionShape2D();
    /// Register object factory.
    static void RegisterObject(Context* context);

    /// Handle enabled/disabled state change.
    virtual void OnSetEnabled();

    /// Set trigger.
    void SetTrigger(bool trigger);
    /// Set filter category bits.
    void SetCategoryBits(int categoryBits);
    /// Set filter mask bits.
    void SetMaskBits(int maskBits);
    /// Set filter group index.
    void SetGroupIndex(int groupIndex);
    /// Set density.
    void SetDensity(float density);
    /// Set friction.
    void SetFriction(float friction);
    /// Set restitution .
    void SetRestitution(float restitution);

    /// Create fixture.
    void CreateFixture();
    /// Release fixture.
    void ReleaseFixture();

    /// Return trigger.
    bool IsTrigger() const { return fixtureDef_.isSensor; }
    /// Return filter category bits.
    int GetCategoryBits() const { return fixtureDef_.filter.categoryBits; }
    /// Return filter mask bits.
    int GetMaskBits() const { return fixtureDef_.filter.maskBits; }
    /// Return filter group index.
    int GetGroupIndex() const { return fixtureDef_.filter.groupIndex; }
    /// Return density.
    float GetDensity() const { return fixtureDef_.density; }
    /// Return friction.
    float GetFriction() const { return fixtureDef_.friction; }
    /// Return restitution.
    float GetRestitution() const { return fixtureDef_.restitution; }
    /// Return mass.
    float GetMass() const;
    /// Return inertia.
    float GetInertia() const;
    /// Return mass center.
    Vector2 GetMassCenter() const;

    /// Return fixture.
    b2Fixture* GetFixture() const { return fixture_;}

protected:
    /// Handle node being assigned.
    virtual void OnNodeSet(Node* node);
    /// Handle node transform being dirtied.
    virtual void OnMarkedDirty(Node* node);
    /// Apply Node world scale.
    virtual void ApplyNodeWorldScale() = 0;

    /// Rigid body.
    WeakPtr<RigidBody2D> rigidBody_;
    /// Fixture def.
    b2FixtureDef fixtureDef_;
    /// Box2D fixture.
    b2Fixture* fixture_;
    /// Cached world scale.
    Vector3 cachedWorldScale_;
};

}
