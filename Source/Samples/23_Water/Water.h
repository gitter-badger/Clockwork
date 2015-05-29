#pragma once

#include <Clockwork/Math/Plane.h>

#include "Sample.h"

namespace Clockwork
{

class Node;
class Scene;

}

/// Water example.
/// This sample demonstrates:
///     - Creating a large plane to represent a water body for rendering
///     - Setting up a second camera to render reflections on the water surface
class Water : public Sample
{
    OBJECT(Water);

public:
    /// Construct.
    Water(Context* context);

    /// Setup after engine initialization and before running the main loop.
    virtual void Start();

private:
    /// Construct the scene content.
    void CreateScene();
    /// Construct an instruction text to the UI.
    void CreateInstructions();
    /// Set up a viewport for displaying the scene.
    void SetupViewport();
    /// Subscribe to the logic update event.
    void SubscribeToEvents();
    /// Read input and moves the camera.
    void MoveCamera(float timeStep);
    /// Handle the logic update event.
    void HandleUpdate(StringHash eventType, VariantMap& eventData);

    /// Reflection camera scene node.
    SharedPtr<Node> reflectionCameraNode_;
    /// Water body scene node.
    SharedPtr<Node> waterNode_;
    /// Reflection plane representing the water surface.
    Plane waterPlane_;
    /// Clipping plane for reflection rendering. Slightly biased downward from the reflection plane to avoid artifacts.
    Plane waterClipPlane_;
};
