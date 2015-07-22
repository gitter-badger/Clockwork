

#pragma once

#include "Sample.h"

namespace Clockwork
{

class Node;
class Scene;

}

class Vehicle;

/// Vehicle example.
/// This sample demonstrates:
///     - Creating a heightmap terrain with collision
///     - Constructing a physical vehicle with rigid bodies for the hull and the wheels, joined with constraints
///     - Defining attributes (including node and component references) of a custom component so that it can be saved and loaded
class VehicleDemo : public Sample
{
    OBJECT(VehicleDemo);

public:
    /// Construct.
    VehicleDemo(Context* context);
    
    /// Setup after engine initialization and before running the main loop.
    virtual void Start();
    
private:
    /// Create static scene content.
    void CreateScene();
    /// Create the vehicle.
    void CreateVehicle();
    /// Construct an instruction text to the UI.
    void CreateInstructions();
    /// Subscribe to necessary events.
    void SubscribeToEvents();
    /// Handle application update. Set controls to vehicle.
    void HandleUpdate(StringHash eventType, VariantMap& eventData);
    /// Handle application post-update. Update camera position after vehicle has moved.
    void HandlePostUpdate(StringHash eventType, VariantMap& eventData);
    
    /// The controllable vehicle component.
    WeakPtr<Vehicle> vehicle_;
};
