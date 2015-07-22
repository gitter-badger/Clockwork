

#pragma once

#include "../Container/Str.h"

#include <cstdlib>

namespace Clockwork
{

class Mutex;

/// Initialize the FPU to round-to-nearest, single precision mode.
CLOCKWORK_API void InitFPU();
/// Display an error dialog with the specified title and message.
CLOCKWORK_API void ErrorDialog(const String& title, const String& message);
/// Exit the application with an error message to the console.
CLOCKWORK_API void ErrorExit(const String& message = String::EMPTY, int exitCode = EXIT_FAILURE);
/// Open a console window.
CLOCKWORK_API void OpenConsoleWindow();
/// Print Unicode text to the console. Will not be printed to the MSVC output window.
CLOCKWORK_API void PrintUnicode(const String& str, bool error = false);
/// Print Unicode text to the console with a newline appended. Will not be printed to the MSVC output window.
CLOCKWORK_API void PrintUnicodeLine(const String& str, bool error = false);
/// Print ASCII text to the console with a newline appended. Uses printf() to allow printing into the MSVC output window.
CLOCKWORK_API void PrintLine(const String& str, bool error = false);
/// Parse arguments from the command line. First argument is by default assumed to be the executable name and is skipped.
CLOCKWORK_API const Vector<String>& ParseArguments(const String& cmdLine, bool skipFirstArgument = true);
/// Parse arguments from the command line.
CLOCKWORK_API const Vector<String>& ParseArguments(const char* cmdLine);
/// Parse arguments from a wide char command line.
CLOCKWORK_API const Vector<String>& ParseArguments(const WString& cmdLine);
/// Parse arguments from a wide char command line.
CLOCKWORK_API const Vector<String>& ParseArguments(const wchar_t* cmdLine);
/// Parse arguments from argc & argv.
CLOCKWORK_API const Vector<String>& ParseArguments(int argc, char** argv);
/// Return previously parsed arguments.
CLOCKWORK_API const Vector<String>& GetArguments();
/// Read input from the console window. Return empty if no input.
CLOCKWORK_API String GetConsoleInput();
/// Return the runtime platform identifier, one of "Windows", "Linux", "Mac OS X", "Android", "iOS" or "Raspberry Pi".
CLOCKWORK_API String GetPlatform();
/// Return the number of physical CPU cores.
CLOCKWORK_API unsigned GetNumPhysicalCPUs();
/// Return the number of logical CPUs (different from physical if hyperthreading is used.)
CLOCKWORK_API unsigned GetNumLogicalCPUs();

}
