
#if defined(WIN32) && !defined(CLOCKWORK_WIN32_CONSOLE)
#include <Clockwork/Core/MiniDump.h>
#include <windows.h>
#ifdef _MSC_VER
#include <crtdbg.h>
#endif
#endif

#include <Clockwork/Core/ProcessUtils.h>
#include <Clockwork/IO/Log.h>

#include "AEApplication.h"
#include "Player/AEPlayerApplication.h"

using namespace ClockworkEditor;

static int RunEditorApplication()
{
    Clockwork::SharedPtr<Clockwork::Context> context(new Clockwork::Context());
    Clockwork::SharedPtr<AEApplication> application(new AEApplication(context));
    return application->Run();
}

static int RunPlayerApplication()
{
    Clockwork::SharedPtr<Clockwork::Context> context(new Clockwork::Context());
    Clockwork::SharedPtr<AEPlayerApplication> application(new AEPlayerApplication(context));
    return application->Run();
}


// Define a platform-specific main function, which in turn executes the user-defined function

// MSVC debug mode: use memory leak reporting
#if defined(_MSC_VER) && defined(_DEBUG) && !defined(CLOCKWORK_WIN32_CONSOLE)
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    Clockwork::ParseArguments(GetCommandLineW());

    const Vector<String>& arguments = GetArguments();

    bool runPlayer = false;
    for (unsigned i = 0; i < arguments.Size();i++)
    {
        if (arguments.At(i) == "--player")
        {
            runPlayer = true;
            break;
        }
    }

    if (runPlayer)
        return RunPlayerApplication();

    return RunEditorApplication();

}
// MSVC release mode: write minidump on crash
#elif defined(_MSC_VER) && defined(CLOCKWORK_MINIDUMPS) && !defined(CLOCKWORK_WIN32_CONSOLE)

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
    Clockwork::ParseArguments(GetCommandLineW());
    int exitCode;
    __try
    {
        exitCode = function;
    }
    __except(Clockwork::WriteMiniDump("Clockwork", GetExceptionInformation()))
    {
    }
    return exitCode;
}
// Other Win32 or minidumps disabled: just execute the function
#elif defined(WIN32) && !defined(CLOCKWORK_WIN32_CONSOLE)
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
    Clockwork::ParseArguments(GetCommandLineW());

    const Vector<String>& arguments = GetArguments();

    bool runPlayer = false;
    for (unsigned i = 0; i < arguments.Size();i++)
    {
        if (arguments.At(i) == "--player")
        {
            runPlayer = true;
            break;
        }
    }

    if (runPlayer)
        return RunPlayerApplication();

    return RunEditorApplication();
}
// Linux or OS X: use main
#else
int main(int argc, char** argv)
{
    Clockwork::ParseArguments(argc, argv);

    const Vector<String>& arguments = GetArguments();

    bool runPlayer = false;
    for (unsigned i = 0; i < arguments.Size();i++)
    {
        if (arguments.At(i) == "--player")
        {
            runPlayer = true;
            break;
        }
    }

    if (runPlayer)
        return RunPlayerApplication();

    return RunEditorApplication();
}
#endif
