// Copyright (c) 2014-2015, THUNDERBEAST GAMES LLC All rights reserved
// Please see LICENSE.md in repository root for license information
// https://github.com/ClockworkGameEngine/ClockworkGameEngine

#include "ClockworkEditor.h"

#include <Clockwork/IO/Log.h>

#include "Subprocess/CESubprocessSystem.h"

#include "CEExternalTool.h"

namespace ClockworkEditor
{

ExternalTool::ExternalTool(Context* context) :
    Object(context)
{
}

ExternalTool::~ExternalTool()
{

}

bool ExternalTool::LaunchInternal(const String& applicationPath, const String& args)
{
    SubprocessSystem* system = GetSubsystem<SubprocessSystem>();

    Vector<String> vargs;
    if (args.Length())
        vargs = args.Split(' ');

    subprocess_ = system->Launch(applicationPath, vargs);

    return subprocess_.NotNull();

}


}
