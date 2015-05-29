#pragma once

#include "Sample.h"

namespace Clockwork
{
    class Node;
    class Scene;
}

// Clockwork2D particle example.
// This sample demonstrates:
//     - Creating a 2D scene with particle
//     - Displaying the scene using the Renderer subsystem
//     - Handling mouse move to move particle
class Clockwork2DParticle : public Sample
{
    OBJECT(Clockwork2DParticle);

public:
    /// Construct.
    Clockwork2DParticle(Context* context);

    /// Setup after engine initialization and before running the main loop.
    virtual void Start();

protected:
    /// Return XML patch instructions for screen joystick layout for a specific sample app, if any.
    virtual String GetScreenJoystickPatchString() const { return
        "<patch>"
        "    <add sel=\"/element/element[./attribute[@name='Name' and @value='Hat0']]\">"
        "        <attribute name=\"Is Visible\" value=\"false\" />"
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
    /// Subscribe to application-wide logic update events.
    void SubscribeToEvents();
    /// Handle mouse move event.
    void HandleMouseMove(StringHash eventType, VariantMap& eventData);

    /// Particle scene node.
    SharedPtr<Node> particleNode_;
};
