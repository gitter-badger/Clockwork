#pragma once

#include "Sample.h"

namespace Clockwork
{

class Node;
class Scene;

}

/// Material animation example.
/// This sample is base on StaticScene, and it demonstrates:
///     - Usage of material shader animation for mush room material
class MaterialAnimation : public Sample
{
    OBJECT(MaterialAnimation);

public:
    /// Construct.
    MaterialAnimation(Context* context);

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
