

#pragma once

#include <Clockwork/Scene/LogicComponent.h>

// All Clockwork classes reside in namespace Clockwork
using namespace Clockwork;

/// Custom logic component for rotating a scene node.
class Rotator : public LogicComponent
{
    OBJECT(Rotator);
    
public:
    /// Construct.
    Rotator(Context* context);
    
    /// Set rotation speed about the Euler axes. Will be scaled with scene update time step.
    void SetRotationSpeed(const Vector3& speed);
    /// Handle scene update. Called by LogicComponent base class.
    virtual void Update(float timeStep);
    
    /// Return rotation speed.
    const Vector3& GetRotationSpeed() const { return rotationSpeed_; }
    
private:
    /// Rotation speed.
    Vector3 rotationSpeed_;
};
