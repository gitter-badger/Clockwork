

#pragma once

#include "../Scene/Component.h"

#include <Box2D/Box2D.h>

namespace Clockwork
{

class RigidBody2D;
class PhysicsWorld2D;

/// 2D physics constraint component.
class CLOCKWORK_API Constraint2D : public Component
{
    OBJECT(Constraint2D);

public:
    /// Construct.
    Constraint2D(Context* context);
    /// Destruct.
    virtual ~Constraint2D();
    /// Register object factory.
    static void RegisterObject(Context* context);

    /// Handle enabled/disabled state change.
    virtual void OnSetEnabled();
    /// Create Joint.
    void CreateJoint();
    /// Release Joint.
    void ReleaseJoint();

    /// Set other rigid body.
    void SetOtherBody(RigidBody2D* body);
    /// Set collide connected.
    void SetCollideConnected(bool collideConnected);
    /// Set attached constriant (for gear).
    void SetAttachedConstraint(Constraint2D* constraint);

    /// Return owner body.
    RigidBody2D* GetOwnerBody() const { return ownerBody_; }

    /// Return other body.
    RigidBody2D* GetOtherBody() const { return otherBody_; }

    /// Return collide connected.
    bool GetCollideConnected() const { return collideConnected_; }

    /// Return attached constraint (for gear).
    Constraint2D* GetAttachedConstraint() const { return attachedConstraint_; }

    /// Return Box2D joint.
    b2Joint* GetJoint() const { return joint_; }

protected:
    /// Handle node being assigned.
    virtual void OnNodeSet(Node* node);
    /// Handle scene being assigned.
    virtual void OnSceneSet(Scene* scene);
    /// Return joint def.
    virtual b2JointDef* GetJointDef() { return 0; };
    /// Recreate joint.
    void RecreateJoint();
    /// Initialize joint def.
    void InitializeJointDef(b2JointDef* jointDef);

    /// Physics world.
    WeakPtr<PhysicsWorld2D> physicsWorld_;
    /// Box2D joint.
    b2Joint* joint_;
    /// Owner body.
    WeakPtr<RigidBody2D> ownerBody_;
    /// Other body.
    WeakPtr<RigidBody2D> otherBody_;
    /// Collide connected.
    bool collideConnected_;
    /// Attached constraint.
    WeakPtr<Constraint2D> attachedConstraint_;
};

}
