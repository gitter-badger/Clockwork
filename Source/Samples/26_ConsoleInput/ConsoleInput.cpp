

#include <Clockwork/Clockwork.h>

#include <Clockwork/UI/Button.h>
#include <Clockwork/Engine/Console.h>
#include <Clockwork/Core/CoreEvents.h>
#include <Clockwork/Engine/Engine.h>
#include <Clockwork/Engine/EngineEvents.h>
#include <Clockwork/Input/Input.h>
#include <Clockwork/IO/Log.h>
#include <Clockwork/Core/ProcessUtils.h>
#include <Clockwork/Math/Random.h>
#include <Clockwork/Core/Timer.h>

#include "ConsoleInput.h"

#include <Clockwork/DebugNew.h>

// Expands to this example's entry-point
DEFINE_APPLICATION_MAIN(ConsoleInput)

// Hunger level descriptions
String hungerLevels[] = {
    "bursting",
    "well-fed",
    "fed",
    "hungry",
    "very hungry",
    "starving"
};

// Clockwork threat level descriptions
String clockworkThreatLevels[] = {
    "Suddenly Clockwork appears from a dark corner of the fish tank",
    "Clockwork seems to have his eyes set on you",
    "Clockwork is homing in on you mercilessly"
};

ConsoleInput::ConsoleInput(Context* context) :
    Sample(context)
{
}

void ConsoleInput::Start()
{
    // Execute base class startup
    Sample::Start();

    // Subscribe to console commands and the frame update
    SubscribeToEvent(E_CONSOLECOMMAND, HANDLER(ConsoleInput, HandleConsoleCommand));
    SubscribeToEvent(E_UPDATE, HANDLER(ConsoleInput, HandleUpdate));

    // Subscribe key down event
    SubscribeToEvent(E_KEYDOWN, HANDLER(ConsoleInput, HandleEscKeyDown));

    // Hide logo to make room for the console
    SetLogoVisible(false);

    // Show the console by default, make it large. Console will show the text edit field when there is at least one
    // subscriber for the console command event
    Console* console = GetSubsystem<Console>();
    console->SetNumRows(GetSubsystem<Graphics>()->GetHeight() / 16);
    console->SetNumBufferedRows(2 * console->GetNumRows());
    console->SetCommandInterpreter(GetTypeName());
    console->SetVisible(true);
    console->GetCloseButton()->SetVisible(false);

    // Show OS mouse cursor
    GetSubsystem<Input>()->SetMouseVisible(true);

    // Open the operating system console window (for stdin / stdout) if not open yet
    OpenConsoleWindow();

    // Initialize game and print the welcome message
    StartGame();

    // Randomize from system clock
    SetRandomSeed(Time::GetSystemTime());
}

void ConsoleInput::HandleConsoleCommand(StringHash eventType, VariantMap& eventData)
{
    using namespace ConsoleCommand;
    if (eventData[P_ID].GetString() == GetTypeName())
        HandleInput(eventData[P_COMMAND].GetString());
}

void ConsoleInput::HandleUpdate(StringHash eventType, VariantMap& eventData)
{
    // Check if there is input from stdin
    String input = GetConsoleInput();
    if (input.Length())
        HandleInput(input);
}

void ConsoleInput::HandleEscKeyDown(StringHash eventType, VariantMap& eventData)
{
    // Unlike the other samples, exiting the engine when ESC is pressed instead of just closing the console
    if (eventData[KeyDown::P_KEY].GetInt() == KEY_ESC)
        engine_->Exit();
}

void ConsoleInput::StartGame()
{
    Print("Welcome to the Clockwork adventure game! You are the newest fish in the tank; your\n"
          "objective is to survive as long as possible. Beware of hunger and the merciless\n"
          "predator cichlid Clockwork, who appears from time to time. Evading Clockwork is easier\n"
          "with an empty stomach. Type 'help' for available commands.");

    gameOn_ = true;
    foodAvailable_ = false;
    eatenLastTurn_ = false;
    numTurns_ = 0;
    hunger_ = 2;
    clockworkThreat_ = 0;
}

void ConsoleInput::EndGame(const String& message)
{
    Print(message);
    Print("Game over! You survived " + String(numTurns_) + " turns.\n"
          "Do you want to play again (Y/N)?");

    gameOn_ = false;
}

void ConsoleInput::Advance()
{
    if (clockworkThreat_ > 0)
    {
        ++clockworkThreat_;
        if (clockworkThreat_ > 3)
        {
            EndGame("Clockwork has eaten you!");
            return;
        }
    }
    else if (clockworkThreat_ < 0)
        ++clockworkThreat_;
    if (clockworkThreat_ == 0 && Random() < 0.2f)
        ++clockworkThreat_;

    if (clockworkThreat_ > 0)
        Print(clockworkThreatLevels[clockworkThreat_ - 1] + ".");

    if ((numTurns_ & 3) == 0 && !eatenLastTurn_)
    {
        ++hunger_;
        if (hunger_ > 5)
        {
            EndGame("You have died from starvation!");
            return;
        }
        else
            Print("You are " + hungerLevels[hunger_] + ".");
    }

    eatenLastTurn_ = false;

    if (foodAvailable_)
    {
        Print("The floating pieces of fish food are quickly eaten by other fish in the tank.");
        foodAvailable_ = false;
    }
    else if (Random() < 0.15f)
    {
        Print("The overhead dispenser drops pieces of delicious fish food to the water!");
        foodAvailable_ = true;
    }

    ++numTurns_;
}

void ConsoleInput::HandleInput(const String& input)
{
    String inputLower = input.ToLower().Trimmed();
    if (inputLower.Empty())
    {
        Print("Empty input given!");
        return;
    }

    if (inputLower == "quit" || inputLower == "exit")
        engine_->Exit();
    else if (gameOn_)
    {
        // Game is on
        if (inputLower == "help")
            Print("The following commands are available: 'eat', 'hide', 'wait', 'score', 'quit'.");
        else if (inputLower == "score")
            Print("You have survived " + String(numTurns_) + " turns.");
        else if (inputLower == "eat")
        {
            if (foodAvailable_)
            {
                Print("You eat several pieces of fish food.");
                foodAvailable_ = false;
                eatenLastTurn_ = true;
                hunger_ -= (hunger_ > 3) ? 2 : 1;
                if (hunger_ < 0)
                {
                    EndGame("You have killed yourself by over-eating!");
                    return;
                }
                else
                    Print("You are now " + hungerLevels[hunger_] + ".");
            }
            else
                Print("There is no food available.");

            Advance();
        }
        else if (inputLower == "wait")
        {
            Print("Time passes...");
            Advance();
        }
        else if (inputLower == "hide")
        {
            if (clockworkThreat_ > 0)
            {
                bool evadeSuccess = hunger_ > 2 || Random() < 0.5f;
                if (evadeSuccess)
                {
                    Print("You hide behind the thick bottom vegetation, until Clockwork grows bored.");
                    clockworkThreat_ = -2;
                }
                else
                    Print("Your movements are too slow; you are unable to hide from Clockwork.");
            }
            else
                Print("There is nothing to hide from.");

            Advance();
        }
        else
            Print("Cannot understand the input '" + input + "'.");
    }
    else
    {
        // Game is over, wait for (y)es or (n)o reply
        if (inputLower[0] == 'y')
            StartGame();
        else if (inputLower[0] == 'n')
            engine_->Exit();
        else
            Print("Please answer 'y' or 'n'.");
    }
}

void ConsoleInput::Print(const String& output)
{
    // Logging appears both in the engine console and stdout
    LOGRAW(output + "\n");
}
