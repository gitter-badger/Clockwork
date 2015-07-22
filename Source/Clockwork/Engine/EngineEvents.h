

#pragma once

#include "../Core/Object.h"

namespace Clockwork
{

/// A command has been entered on the console.
EVENT(E_CONSOLECOMMAND, ConsoleCommand)
{
    PARAM(P_COMMAND, Command);              // String
    PARAM(P_ID, Id);                        // String
}

}
