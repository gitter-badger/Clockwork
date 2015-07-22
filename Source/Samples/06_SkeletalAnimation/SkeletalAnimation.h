

#pragma once

#include "Sample.h"

namespace Clockwork
{

class Node;
class Scene;

}

/// Skeletal animation example.
/// This sample demonstrates:
///     - Populating a 3D scene with skeletally animated AnimatedModel components;
///     - Moving the animated models and advancing their animation using a custom component
///     - Enabling a cascaded shadow map on a directional light, which allows high-quality shadows
///       over a large area (typically used in outdoor scenes for shadows cast by sunlight)
///     - Displaying renderer debug geometry
class SkeletalAnimation : public Sample
{
    OBJECT(SkeletalAnimation);

public:
    /// Construct.
    SkeletalAnimation(Context* context);

    /// Setup after engine initialization and before running the main loop.
    virtual void Start();

protected:
    /// Return XML patch instructions for screen joystick layout for a specific sample app, if any.
    virtual String GetScreenJoystickPatchString() const { return
        "<patch>"
        "    <remove sel=\"/element/element[./attribute[@name='Name' and @value='Button1']]/attribute[@name='Is Visible']\" />"
        "    <replace sel=\"/element/element[./attribute[@name='Name' and @value='Button1']]/element[./attribute[@name='Name' and @value='Label']]/attribute[@name='Text']/@value\">Debug</replace>"
        "    <add sel=\"/element/element[./attribute[@name='Name' and @value='Button1']]\">"
        "        <element type=\"Text\">"
        "            <attribute name=\"Name\" value=\"KeyBinding\" />"
        "            <attribute name=\"Text\" value=\"SPACE\" />"
        "        </element>"
        "    </add>"
        "</patch>";
    }

private:
    /// Construct the scene content.
    void CreateScene();
    /// Construct an instruction text to the UI.
    void CreateInstructions();
    /// Set up a viewport for displaying the scene.
    void SetupViewport();
    /// Subscribe to application-wide logic update and post-render update events.
    void SubscribeToEvents();
    /// Read input and moves the camera.
    void MoveCamera(float timeStep);
    /// Handle the logic update event.
    void HandleUpdate(StringHash eventType, VariantMap& eventData);
    /// Handle the post-render update event.
    void HandlePostRenderUpdate(StringHash eventType, VariantMap& eventData);
    
    /// Flag for drawing debug geometry.
    bool drawDebug_;
};
