#pragma once

#include "Sample.h"

/// Console input example.
/// This sample demonstrates:
///     - Implementing a crude text adventure game, which accepts input both through the engine console,
///       and standard input.
class ConsoleInput : public Sample
{
    OBJECT(ConsoleInput);

public:
    /// Construct.
    ConsoleInput(Context* context);

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
    /// Handle console command event.
    void HandleConsoleCommand(StringHash eventType, VariantMap& eventData);
    /// Handle frame update event.
    void HandleUpdate(StringHash eventType, VariantMap& eventData);
    /// Handle ESC key down event to quit the engine.
    void HandleEscKeyDown(StringHash eventType, VariantMap& eventData);
    /// Print intro message and initialize the game state.
    void StartGame();
    /// Print game over message.
    void EndGame(const String& message);
    /// Advance the game state.
    void Advance();
    /// Handle user input either from the engine console or standard input.
    void HandleInput(const String& input);
    /// Print text to the engine console and standard output.
    void Print(const String& output);

    /// Game on flag.
    bool gameOn_;
    /// Food dispensed flag.
    bool foodAvailable_;
    /// Whether ate on the previous turn.
    bool eatenLastTurn_;
    /// Number of turns survived.
    int numTurns_;
    /// Player's hunger level.
    int hunger_;
    /// Threat of Clockwork level.
    int clockworkThreat_;
};
