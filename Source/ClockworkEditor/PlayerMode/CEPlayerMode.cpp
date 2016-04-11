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

#include <Clockwork/IO/IOEvents.h>
#include <Clockwork/IO/Log.h>
#include <Clockwork/Input/InputEvents.h>
#include <Clockwork/Core/ProcessUtils.h>
#include <Clockwork/Graphics/GraphicsEvents.h>
#include <Clockwork/Graphics/Graphics.h>
#include <Clockwork/Graphics/Camera.h>
#include <Clockwork/UI/SystemUI/DebugHud.h>
#include <Clockwork/UI/SystemUI/SystemUIEvents.h>
#include <Clockwork/UI/UI.h>
#include <Clockwork/IPC/IPCEvents.h>
#include <Clockwork/IPC/IPCWorker.h>

#include <ClockworkJS/Javascript/JSVM.h>
#include <ClockworkJS/Javascript/JSEvents.h>
#include <ClockworkJS/Javascript/JSIPCEvents.h>

#include "CEPlayerEvents.h"

#include "CEPlayerMode.h"

#ifdef CLOCKWORK_PLATFORM_WINDOWS
#include <windows.h>
#endif

namespace ClockworkEditor
{

PlayerMode::PlayerMode(Context* context) :
    Object(context),
    brokerActive_(false),
    launchedByEditor_(false),
    licenseModule3D_(false)
{
    fd_[0] = INVALID_IPCHANDLE_VALUE;
    fd_[1] = INVALID_IPCHANDLE_VALUE;

    ipc_ = GetSubsystem<IPC>();

    SubscribeToEvent(E_LOGMESSAGE, HANDLER(PlayerMode, HandleLogMessage));
    SubscribeToEvent(E_JSERROR, HANDLER(PlayerMode, HandleJSError));
    SubscribeToEvent(E_EXITREQUESTED, HANDLER(PlayerMode, HandleExitRequest));
    SubscribeToEvent(E_SCREENMODE, HANDLER(PlayerMode, HandlePlayerWindowChanged));
    SubscribeToEvent(E_WINDOWPOS, HANDLER(PlayerMode, HandlePlayerWindowChanged));

    // BEGIN LICENSE MANAGEMENT
    SubscribeToEvent(E_BEGINVIEWRENDER, HANDLER(PlayerMode, HandleViewRender));
    // END LICENSE MANAGEMENT
}

PlayerMode::~PlayerMode()
{

}

void PlayerMode::HandleIPCInitialize(StringHash eventType, VariantMap& eventData)
{
    brokerActive_ = true;

    JSVM* vm = JSVM::GetJSVM(0);

    if (!vm->ExecuteMain())
    {
        SendEvent(E_EXITREQUESTED);
    }

    // BEGIN LICENSE MANAGEMENT

    licenseModule3D_ = eventData["license3D"].GetBool();

    // END LICENSE MANAGEMENT

    SystemUI::DebugHud* debugHud = GetSubsystem<SystemUI::DebugHud>();
    if (debugHud)
        debugHud->SetMode(eventData["debugHudMode"].GetUInt());

}

void PlayerMode::ProcessArguments() {

    const Vector<String>& arguments = GetArguments();

    int id = -1;

    for (unsigned i = 0; i < arguments.Size(); ++i)
    {
        if (arguments[i].Length() > 1)
        {
            String argument = arguments[i].ToLower();
            // String value = i + 1 < arguments.Size() ? arguments[i + 1] : String::EMPTY;

            if (argument.StartsWith("--ipc-id="))
            {
                Vector<String> idc = argument.Split(argument.CString(), '=');
                if (idc.Size() == 2)

                    id = ToInt(idc[1].CString());
            }

            else if (argument.StartsWith("--ipc-server=") || argument.StartsWith("--ipc-client="))
            {
                LOGINFOF("Starting IPCWorker %s", argument.CString());

                Vector<String> ipc = argument.Split(argument.CString(), '=');

                if (ipc.Size() == 2)
                {
                    if (argument.StartsWith("--ipc-server="))
                    {
#ifdef CLOCKWORK_PLATFORM_WINDOWS
                        // clientRead
                        WString wipc(ipc[1]);
                        HANDLE pipe = reinterpret_cast<HANDLE>(_wtoi64(wipc.CString()));
                        fd_[0] = pipe;
#else
                        int fd = ToInt(ipc[1].CString());
                        fd_[0] = fd;
#endif
                    }
                    else
                    {
#ifdef CLOCKWORK_PLATFORM_WINDOWS
                        // clientWrite
                        WString wipc(ipc[1]);
                        HANDLE pipe = reinterpret_cast<HANDLE>(_wtoi64(wipc.CString()));
                        fd_[1] = pipe;
#else
                        int fd = ToInt(ipc[1].CString());
                        fd_[1] = fd;
#endif
                    }

                }

            }

        }
    }

    if (id > 0 && fd_[0] != INVALID_IPCHANDLE_VALUE && fd_[1] != INVALID_IPCHANDLE_VALUE)
    {
        launchedByEditor_ = true;
        SubscribeToEvent(E_IPCINITIALIZE, HANDLER(PlayerMode, HandleIPCInitialize));
        ipc_->InitWorker((unsigned) id, fd_[0], fd_[1]);
    }
}

void PlayerMode::HandleJSError(StringHash eventType, VariantMap& eventData)
{
    if (brokerActive_)
    {
        if (ipc_.Null())
            return;

        String errName = eventData[JSError::P_ERRORNAME].GetString();
        String errStack = eventData[JSError::P_ERRORSTACK].GetString();
        String errMessage = eventData[JSError::P_ERRORMESSAGE].GetString();
        String errFilename = eventData[JSError::P_ERRORFILENAME].GetString();
        int errLineNumber = eventData[JSError::P_ERRORLINENUMBER].GetInt();

        VariantMap ipcErrorData;
        ipcErrorData[IPCJSError::P_ERRORNAME] = errName;
        ipcErrorData[IPCJSError::P_ERRORSTACK] = errStack;
        ipcErrorData[IPCJSError::P_ERRORMESSAGE] = errMessage;
        ipcErrorData[IPCJSError::P_ERRORFILENAME] = errFilename;
        ipcErrorData[IPCJSError::P_ERRORLINENUMBER] = errLineNumber;

        ipc_->SendEventToBroker(E_IPCJSERROR, ipcErrorData);

        LOGERROR("SENDING E_IPCJSERROR");

    }

}


void PlayerMode::HandleLogMessage(StringHash eventType, VariantMap& eventData)
{
    using namespace LogMessage;

    if (brokerActive_)
    {

        if (ipc_.Null())
            return;

        VariantMap logEvent;
        logEvent[IPCWorkerLog::P_LEVEL] = eventData[P_LEVEL].GetInt();
        logEvent[IPCWorkerLog::P_MESSAGE] = eventData[P_MESSAGE].GetString();
        ipc_->SendEventToBroker(E_IPCWORKERLOG, logEvent);
    }

}

void PlayerMode::HandleMessageAck(StringHash eventType, VariantMap& eventData)
{
    messageBox_ = 0;
    GetSubsystem<UI>()->RequestExit();
}

void PlayerMode::HandleViewRender(StringHash eventType, VariantMap& eventData)
{
// BEGIN LICENSE MANAGEMENT
    static bool done = false;

    if (!launchedByEditor_ || licenseModule3D_)
        return;

    Camera* camera = static_cast<Camera*>(eventData[BeginViewRender::P_CAMERA].GetPtr());

    if (!camera || camera->IsOrthographic())
        return;

    if (!done) {

        done = true;

        messageBox_ = GetSubsystem<UI>()->ShowSystemMessageBox("3D Module License Required", "A 3D Module License is required to display 3D content.\n\nUpgrade to Clockwork Pro for all features and platforms.");
        SubscribeToEvent(messageBox_, SystemUI::E_MESSAGEACK, HANDLER(PlayerMode, HandleMessageAck));

        if (brokerActive_)
        {

            if (ipc_.Null())
                return;

            VariantMap msgEvent;
            msgEvent[IPCMessage::P_MESSAGE] = String("3D Module License Required");
            ipc_->SendEventToBroker(E_IPCMESSAGE, msgEvent);
        }
    }

// END LICENSE MANAGEMENT

}

void PlayerMode::HandlePlayerWindowChanged(StringHash eventType, VariantMap& eventData)
{
    Graphics* graphics = GetSubsystem<Graphics>();
    using namespace IPCPlayerWindowChanged;
    VariantMap data;
    data[P_POSX] = graphics->GetWindowPosition().x_;
    data[P_POSY] = graphics->GetWindowPosition().y_;
    data[P_WIDTH] = graphics->GetWidth();
    data[P_HEIGHT] = graphics->GetHeight();
    data[P_MONITOR] = graphics->GetCurrentMonitor();
    data[P_MAXIMIZED] = graphics->GetMaximized();
    ipc_->SendEventToBroker(E_IPCPLAYERWINDOWCHANGED, data);
}

void PlayerMode::HandleExitRequest(StringHash eventType, VariantMap& eventData)
{
    UnsubscribeFromEvent(E_LOGMESSAGE);
    SendEvent(E_PLAYERQUIT);
}

}