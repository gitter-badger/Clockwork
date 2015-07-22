

#pragma once

#include "Sample.h"

namespace Clockwork
{

class Button;
class Scene;
class Slider;

}

/// Sound effects example
/// This sample demonstrates:
///     - Playing sound effects and music
///     - Controlling sound and music master volume
class SoundEffects : public Sample
{
    OBJECT(SoundEffects);

public:
    /// Construct.
    SoundEffects(Context* context);

    /// Setup after engine initialization and before running the main loop.
    virtual void Start();

protected:
    /// Return XML patch instructions for screen joystick layout for a specific sample app, if any.
    virtual String GetScreenJoystickPatchString() const { return
        "<patch>"
        "    <add sel=\"/element/element[./attribute[@name='Name' and @value='Button2']]\">"
        "        <attribute name=\"Is Visible\" value=\"false\" />"
        "    </add>"
        "    <add sel=\"/element/element[./attribute[@name='Name' and @value='Hat0']]\">"
        "        <attribute name=\"Is Visible\" value=\"false\" />"
        "    </add>"
        "</patch>";
    }

private:
    /// Create the UI and subscribes to UI events.
    void CreateUI();
    /// Create a button at position with specified text in it.
    Button* CreateButton(int x, int y, int xSize, int ySize, const String& text);
    /// Create a horizontal slider with specified text above it.
    Slider* CreateSlider(int x, int y, int xSize, int ySize, const String& text);
    /// Handle a sound effect button click.
    void HandlePlaySound(StringHash eventType, VariantMap& eventData);
    /// Handle "play music" button click.
    void HandlePlayMusic(StringHash eventType, VariantMap& eventData);
    /// Handle "stop music" button click.
    void HandleStopMusic(StringHash eventType, VariantMap& eventData);
    /// Handle sound effects volume slider change.
    void HandleSoundVolume(StringHash eventType, VariantMap& eventData);
    /// Handle music volume slider change.
    void HandleMusicVolume(StringHash eventType, VariantMap& eventData);
};


