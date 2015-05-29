#pragma once

#include "../Core/ProcessUtils.h"

#if defined(WIN32) && !defined(CLOCKWORK_WIN32_CONSOLE)
#include "../Core/MiniDump.h"
#include <windows.h>
#ifdef _MSC_VER
#include <crtdbg.h>
#endif
#endif

// Define a platform-specific main function, which in turn executes the user-defined function

// MSVC debug mode: use memory leak reporting
#if defined(_MSC_VER) && defined(_DEBUG) && !defined(CLOCKWORK_WIN32_CONSOLE)
#define DEFINE_MAIN(function) \
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd) \
{ \
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); \
    Clockwork::ParseArguments(GetCommandLineW()); \
    return function; \
}
// MSVC release mode: write minidump on crash
#elif defined(_MSC_VER) && defined(CLOCKWORK_MINIDUMPS) && !defined(CLOCKWORK_WIN32_CONSOLE)
#define DEFINE_MAIN(function) \
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd) \
{ \
    Clockwork::ParseArguments(GetCommandLineW()); \
    int exitCode; \
    __try \
    { \
        exitCode = function; \
    } \
    __except(Clockwork::WriteMiniDump("Clockwork", GetExceptionInformation())) \
    { \
    } \
    return exitCode; \
}
// Other Win32 or minidumps disabled: just execute the function
#elif defined(WIN32) && !defined(CLOCKWORK_WIN32_CONSOLE)
#define DEFINE_MAIN(function) \
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd) \
{ \
    Clockwork::ParseArguments(GetCommandLineW()); \
    return function; \
}
// Android or iOS: use SDL_main
#elif defined(ANDROID) || defined(IOS)
#define DEFINE_MAIN(function) \
extern "C" int SDL_main(int argc, char** argv); \
int SDL_main(int argc, char** argv) \
{ \
    Clockwork::ParseArguments(argc, argv); \
    return function; \
}
// Linux or OS X: use main
#else
#define DEFINE_MAIN(function) \
int main(int argc, char** argv) \
{ \
    Clockwork::ParseArguments(argc, argv); \
    return function; \
}
#endif
