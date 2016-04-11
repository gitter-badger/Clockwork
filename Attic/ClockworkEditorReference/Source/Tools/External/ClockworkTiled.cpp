// Copyright (c) 2014-2015, THUNDERBEAST GAMES LLC All rights reserved
// Please see LICENSE.md in repository root for license information
// https://github.com/ClockworkGameEngine/ClockworkGameEngine

#include "ClockworkEditor.h"

#include <Clockwork/IO/Log.h>

#include "CEEvents.h"
#include "CEExternalTooling.h"

#include "ClockworkTiled.h"

namespace ClockworkEditor
{

ClockworkTiled::ClockworkTiled(Context* context) :
    ExternalTool(context)
{
}

ClockworkTiled::~ClockworkTiled()
{

}

void ClockworkTiled::Open(const String& fullpath)
{

}

void ClockworkTiled::HandleEvent(StringHash eventType, VariantMap& eventData)
{
    if (eventType == E_SUBPROCESSOUTPUT)
    {
        String output = eventData[SubprocessOutput::P_TEXT].GetString();
        LOGINFOF("TILED: %s", output.CString());

    }
    else if (eventType == E_SUBPROCESSCOMPLETE)
    {
        ExternalTooling* tooling = GetSubsystem<ExternalTooling>();
        tooling->ToolExited("ClockworkTiled");
    }
}

bool ClockworkTiled::Launch(const String& args)
{
    ExternalTooling* tooling = GetSubsystem<ExternalTooling>();
    String app = tooling->GetToolApplicationPath();

 #ifdef CLOCKWORK_PLATFORM_WINDOWS
    app += "ClockworkTiled/tiled.exe";
 #else
    app += "Tiled.app/Contents/MacOS/Tiled";

 #endif

    LaunchInternal(app, args);

    if (subprocess_)
    {
        SubscribeToEvent(subprocess_, E_SUBPROCESSCOMPLETE, HANDLER(ClockworkTiled, HandleEvent));
        SubscribeToEvent(subprocess_, E_SUBPROCESSOUTPUT, HANDLER(ClockworkTiled, HandleEvent));
        return true;
    }

    return false;

}

}
