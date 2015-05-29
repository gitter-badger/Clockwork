#pragma once

#include "../Core/Object.h"

namespace Clockwork
{

/// Log message event.
EVENT(E_LOGMESSAGE, LogMessage)
{
    PARAM(P_MESSAGE, Message);              // String
    PARAM(P_LEVEL, Level);                  // int
}

/// Async system command execution finished.
EVENT(E_ASYNCEXECFINISHED, AsyncExecFinished)
{
    PARAM(P_REQUESTID, RequestID);          // unsigned
    PARAM(P_EXITCODE, ExitCode);            // int
}

}
