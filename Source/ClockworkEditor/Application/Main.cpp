//
// Copyright (c) 2014-2016 CLOCKWORK STUDIOS
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

#if defined(CLOCKWORK_PLATFORM_WINDOWS) || defined (CLOCKWORK_PLATFORM_LINUX)
#ifdef CLOCKWORK_WEBVIEW
#include <ClockworkWebView/ClockworkWebView.h>
#endif
#endif

#if defined(WIN32) && !defined(CLOCKWORK_WIN32_CONSOLE)
#include <Clockwork/Core/MiniDump.h>
#include <windows.h>
#ifdef _MSC_VER
#include <crtdbg.h>
#endif
#endif

#include <Clockwork/Core/ProcessUtils.h>
#include <Clockwork/IO/Log.h>

#include "CEEditorApp.h"
#include "CEPlayerApp.h"

using namespace ClockworkEditor;

static int RunEditorApplication()
{
    Clockwork::SharedPtr<Clockwork::Context> context(new Clockwork::Context());
    Clockwork::SharedPtr<CEEditorApp> application(new CEEditorApp(context));
    return application->Run();
}

static int RunPlayerApplication()
{
    Clockwork::SharedPtr<Clockwork::Context> context(new Clockwork::Context());
    Clockwork::SharedPtr<CEPlayerApplication> application(new CEPlayerApplication(context));
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

#if defined(CLOCKWORK_PLATFORM_WINDOWS) || defined (CLOCKWORK_PLATFORM_LINUX)
#ifdef CLOCKWORK_WEBVIEW

    int exit_code = Clockwork::WebMain(argc, argv);

    if (exit_code >= 0)
    {
        // The sub-process has completed so return here.
        return exit_code;
    }

#endif
#endif

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
