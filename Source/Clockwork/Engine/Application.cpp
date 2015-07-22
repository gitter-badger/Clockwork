

#include "../Precompiled.h"

#include "../Engine/Application.h"
#include "../Engine/Engine.h"
#ifdef IOS
#include "../Graphics/Graphics.h"
#include "../Graphics/GraphicsImpl.h"
#endif
#include "../IO/IOEvents.h"
#include "../IO/Log.h"

#include "../DebugNew.h"

namespace Clockwork
{

#if defined(IOS) || defined(EMSCRIPTEN)
// Code for supporting SDL_iPhoneSetAnimationCallback() and emscripten_set_main_loop_arg()
#if defined(EMSCRIPTEN)
#include <emscripten.h>
#endif
void RunFrame(void* data)
{
    static_cast<Engine*>(data)->RunFrame();
}
#endif

Application::Application(Context* context) :
    Object(context),
    exitCode_(EXIT_SUCCESS)
{
    engineParameters_ = Engine::ParseParameters(GetArguments());

    // Create the Engine, but do not initialize it yet. Subsystems except Graphics & Renderer are registered at this point
    engine_ = new Engine(context);

    // Subscribe to log messages so that can show errors if ErrorExit() is called with empty message
    SubscribeToEvent(E_LOGMESSAGE, HANDLER(Application, HandleLogMessage));
}

int Application::Run()
{
    // Emscripten-specific: C++ exceptions are turned off by default in -O1 (and above), unless '-s DISABLE_EXCEPTION_CATCHING=0' flag is set
    // Clockwork build configuration uses -O3 (Release), -O2 (RelWithDebInfo), and -O0 (Debug)
    // Thus, the try-catch block below should be optimised out except in Debug build configuration
    try
    {
        Setup();
        if (exitCode_)
            return exitCode_;

        if (!engine_->Initialize(engineParameters_))
        {
            ErrorExit();
            return exitCode_;
        }

        Start();
        if (exitCode_)
            return exitCode_;

        // Platforms other than iOS and EMSCRIPTEN run a blocking main loop
#if !defined(IOS) && !defined(EMSCRIPTEN)
        while (!engine_->IsExiting())
            engine_->RunFrame();

        Stop();
        // iOS will setup a timer for running animation frames so eg. Game Center can run. In this case we do not
        // support calling the Stop() function, as the application will never stop manually
#else
#if defined(IOS)
        SDL_iPhoneSetAnimationCallback(GetSubsystem<Graphics>()->GetImpl()->GetWindow(), 1, &RunFrame, engine_);
#elif defined(EMSCRIPTEN)
        emscripten_set_main_loop_arg(RunFrame, engine_, 0, 1);
#endif
#endif

        return exitCode_;
    }
    catch (std::bad_alloc&)
    {
        ErrorDialog(GetTypeName(), "An out-of-memory error occurred. The application will now exit.");
        return EXIT_FAILURE;
    }
}

void Application::ErrorExit(const String& message)
{
    engine_->Exit(); // Close the rendering window
    exitCode_ = EXIT_FAILURE;

    // Only for WIN32, otherwise the error messages would be double posted on Mac OS X and Linux platforms
    if (!message.Length())
    {
#ifdef WIN32
        ErrorDialog(GetTypeName(), startupErrors_.Length() ? startupErrors_ :
            "Application has been terminated due to unexpected error.");
#endif
    }
    else
        ErrorDialog(GetTypeName(), message);
}

void Application::HandleLogMessage(StringHash eventType, VariantMap& eventData)
{
    using namespace LogMessage;

    if (eventData[P_LEVEL].GetInt() == LOG_ERROR)
    {
        // Strip the timestamp if necessary
        String error = eventData[P_MESSAGE].GetString();
        unsigned bracketPos = error.Find(']');
        if (bracketPos != String::NPOS)
            error = error.Substring(bracketPos + 2);

        startupErrors_ += error + "\n";
    }
}


}
