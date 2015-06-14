#pragma once

#include "../Scene/Component.h"
#include <Box2D/Box2D.h>

namespace Clockwork
{

class CollisionShape2D;
class Constraint2D;
class PhysicsWorld2D;

/// Rigid body type.
enum BodyType2D
{
    BT_STATIC = b2_staticBody,
    BT_DYNAMIC = b2_dynamicBody,
    BT_KINEMATIC = b2_kinematicBody,
};

/// 2D rigid body component.
class CLOCKWORK_API RigidBody2D : public Component
{
    OBJECT(RigidBody2D);

public:
    /// Construct.
    RigidBody2D(Context* scontext);
    /// Destruct.
    virtual ~RigidBody2D();
    /// Register object factory.
    static void RegisterObject(Context* context);

    /// Handle enabled/disabled state change.
    virtual void OnSetEnabled();

    /// Set body type.
    void SetBodyType(BodyType2D bodyType);
    /// Set Mass.
    void SetMass(float mass);
    /// Set inertia.
    void SetInertia(float inertia);
    /// Set mass center.
    void SetMassCenter(const Vector2& center);
    /// Use fixture mass (default is true).
    void SetUseFixtureMass(bool useFixtureMass);
    /// Set linear damping.
    void SetLinearDamping(float linearDamping);
    /// Set angular damping.
    void SetAngularDamping(float angularDamping);
    /// Set allow sleep.
    void SetAllowSleep(bool allowSleep);
    /// Set fixed rotation.
    void SetFixedRotation(bool fixedRotation);
    /// Set bullet.
    void SetBullet(bool bullet);
    /// Set gravity scale.
    void SetGravityScale(float gravityScale);
    /// Set awake.
    void SetAwake(bool awake);
    /// Set linear velocity.
    void SetLinearVelocity(const Vector2& linearVelocity);
    /// Set angular velocity.
    void SetAngularVelocity(float angularVelocity);
    /// Apply force.
    void ApplyForce(const Vector2& force, const Vector2& point,  bool wake);
    /// Apply force to center.
    void ApplyForceToCenter(const Vector2& force, bool wake);
    /// Apply Torque.
    void ApplyTorque(float torque, bool wake);
    /// Apply linear impulse.
    void ApplyLinearImpulse(const Vector2& impulse, const Vector2& point, bool wake);
    /// Apply angular impulse.
    void ApplyAngularImpulse(float impulse, bool wake);

    /// Create body.
    void CreateBody();
    /// Release body.
    void ReleaseBody();

    /// Apply world transform.
    void ApplyWorldTransform();
    /// Add collision shape.
    void AddCollisionShape2D(CollisionShape2D* collisionShape);
    /// Remove collision shape.
    void RemoveCollisionShape2D(CollisionShape2D* collisionShape);
    /// Add constraint.
    void AddConstraint2D(Constraint2D* constraint);
    /// Remove constraint.
    void RemoveConstraint2D(Constraint2D* constraint);

    /// Return body type.
    BodyType2D GetBodyType() const { return (BodyType2D)bodyDef_.type; }
    /// Return Mass.
    float GetMass() const;
    /// Return inertia.
    float GetInertia() const;
    /// Return mass center.
    Vector2 GetMassCenter() const;
    /// Return use fixture mass.
    bool GetUseFixtureMass() const { return useFixtureMass_; }
    /// Return linear damping.
    float GetLinearDamping() const { return bodyDef_.linearDamping; }
    /// Return angular damping.
    float GetAngularDamping() const { return bodyDef_.angularDamping; }
    /// Return allow sleep.
    bool IsAllowSleep() const { return bodyDef_.allowSleep; }
    /// Return fixed rotation.
    bool IsFixedRotation() const { return bodyDef_.fixedRotation; }
    /// Return bullet.
    bool IsBullet() const { return bodyDef_.bullet; }
    /// Return gravity scale.
    float GetGravityScale() const { return bodyDef_.gravityScale; }
    /// Return awake.
    bool IsAwake() const;
    /// Return linear velocity.
    Vector2 GetLinearVelocity() const;
    /// Return angular velocity.
    float GetAngularVelocity() const;

    /// Return Box2D body.
    b2Body* GetBody() const { return body_; }

private:
    /// Handle node being assigned.
    virtual void OnNodeSet(Node* node);
    /// Handle node transform being dirtied.
    virtual void OnMarkedDirty(Node* node);

    /// Physics world.
    WeakPtr<PhysicsWorld2D> physicsWorld_;
    /// Box2D body define.
    b2BodyDef bodyDef_;
    /// Box2D mass data.
    b2MassData massData_;
    /// Use fixture mass.
    bool useFixtureMass_;
    /// Box2D body.
    b2Body* body_;
    /// Collision shapes.
    Vector<WeakPtr<CollisionShape2D> > collisionShapes_;
    /// Constraints.
    Vector<WeakPtr<Constraint2D> > constraints_;
};

}