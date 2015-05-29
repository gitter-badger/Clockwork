#pragma once

#include "Sample.h"

namespace Clockwork
{

class Node;
class Scene;

}

/// Signed distance field text example.
/// This sample demonstrates:
///     - Creating a 3D scene with static content
///     - Creating a 3D text use SDF Font
///     - Displaying the scene using the Renderer subsystem
///     - Handling keyboard and mouse input to move a freelook camera
class SignedDistanceFieldText : public Sample
{
    OBJECT(SignedDistanceFieldText);

public:
    /// Construct.
    SignedDistanceFieldText(Context* context);

    /// Setup after engine initialization and before running the main loop.
    virtual void Start();

private:
    /// Construct the scene content.
    void CreateScene();
    /// Construct an instruction text to the UI.
    void CreateInstructions();
    /// Set up a viewport for displaying the scene.
    void SetupViewport();
    /// Read input and moves the camera.
    void MoveCamera(float timeStep);
    /// Subscribe to application-wide logic update events.
    void SubscribeToEvents();
    /// Handle the logic update event.
    void HandleUpdate(StringHash eventType, VariantMap& eventData);
};
