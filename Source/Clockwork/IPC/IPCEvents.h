//
// Copyright (c) 2014-2015, CLOCKWORK STUDIOS All rights reserved
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

#pragma once

#include "../Core/Object.h"

namespace Clockwork
{

/// Worker start
EVENT(E_IPCWORKERSTART, IPCWorkerStart)
{

}

/// Worker exited
EVENT(E_IPCWORKEREXIT, IPCWorkerExit)
{
    PARAM(P_BROKER, Broker);   // Broker*
    PARAM(P_EXITCODE, ExitCode);   // int
}

/// broker -> worker start
EVENT(E_IPCINITIALIZE, IPCInitialize)
{

}

/// Worker Log
EVENT(E_IPCWORKERLOG, IPCWorkerLog)
{
    PARAM(P_LEVEL, Level);      // int log level
    PARAM(P_MESSAGE, Message);  // string
}

/// Worker Log
EVENT(E_IPCMESSAGE, IPCMessage)
{
    PARAM(P_MESSAGE, Message);  // string
    PARAM(P_VALUE, Value);  // int
}


}