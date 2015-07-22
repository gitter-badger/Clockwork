

#pragma once

#include "Sample.h"

namespace Clockwork
{
    class Node;
    class Scene;
}

/// GUI test example.
/// This sample demonstrates:
///     - Creating GUI elements from C++
///     - Loading GUI Style from xml
///     - Subscribing to GUI drag events and handling them.
class UIDrag : public Sample
{
    OBJECT(UIDrag);

public:
    /// Construct.
    UIDrag(Context* context);

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
    /// Construct the GUI.
    void CreateGUI();
    /// Construct an instruction text to the UI.
    void CreateInstructions();
    /// Subscribe to application-wide logic update events.
    void SubscribeToEvents();

    void HandleUpdate(StringHash eventType, VariantMap& eventData);
    void HandleDragBegin(StringHash eventType, VariantMap& eventData);
    void HandleDragMove(StringHash eventType, VariantMap& eventData);
    void HandleDragCancel(StringHash eventType, VariantMap& eventData);
    void HandleDragEnd(StringHash eventType, VariantMap& eventData);
    void HandleTextFinished(StringHash eventType, VariantMap& eventData);
    void HandleLineEditDragBegin(StringHash eventType, VariantMap& eventData);
};
