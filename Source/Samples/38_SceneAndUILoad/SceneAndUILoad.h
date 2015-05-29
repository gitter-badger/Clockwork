#pragma once

#include "Sample.h"

namespace Clockwork
{

class Drawable;
class Node;
class Scene;

}

/// Scene & UI load example.
/// This sample demonstrates:
///      - Loading a scene from a file and showing it
///      - Loading a UI layout from a file and showing it
///      - Subscribing to the UI layout's events
class SceneAndUILoad : public Sample
{
    OBJECT(SceneAndUILoad);

public:
    /// Construct.
    SceneAndUILoad(Context* context);

    /// Setup after engine initialization and before running the main loop.
    virtual void Start();

private:
    /// Construct the scene content.
    void CreateScene();
    /// Construct user interface elements.
    void CreateUI();
    /// Set up a viewport for displaying the scene.
    void SetupViewport();
    /// Subscribe to application-wide logic update event.
    void SubscribeToEvents();
    /// Reads input and moves the camera.
    void MoveCamera(float timeStep);
    /// Handle the logic update event.
    void HandleUpdate(StringHash eventType, VariantMap& eventData);
    /// Handle toggle button 1 being pressed.
    void ToggleLight1(StringHash eventType, VariantMap& eventData);
    /// Handle toggle button 2 being pressed.
    void ToggleLight2(StringHash eventType, VariantMap& eventData);
};
