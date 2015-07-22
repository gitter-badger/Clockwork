

#pragma once

#include <Clockwork/Physics/CollisionShape.h>
#include <Clockwork/Physics/Constraint.h>

using namespace Clockwork;

/// Custom component that creates a ragdoll upon collision.
class CreateRagdoll : public Component
{
    OBJECT(CreateRagdoll);
    
public:
    /// Construct.
    CreateRagdoll(Context* context);
    
protected:
    /// Handle node being assigned.
    virtual void OnNodeSet(Node* node);
    
private:
    /// Handle scene node's physics collision.
    void HandleNodeCollision(StringHash eventType, VariantMap& eventData);
    /// Make a bone physical by adding RigidBody and CollisionShape components.
    void CreateRagdollBone(const String& boneName, ShapeType type, const Vector3& size, const Vector3& position, const Quaternion& rotation);
    /// Join two bones with a Constraint component.
    void CreateRagdollConstraint(const String& boneName, const String& parentName, ConstraintType type, const Vector3& axis, const Vector3& parentAxis, const Vector2& highLimit, const Vector2& lowLimit, bool disableCollision = true);
};
