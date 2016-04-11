// Copyright (c) 2014-2015, THUNDERBEAST GAMES LLC All rights reserved
// Please see LICENSE.md in repository root for license information
// https://github.com/ClockworkGameEngine/ClockworkGameEngine

#pragma once

#include <Clockwork/Core/Object.h>
#include "Subprocess/CESubprocess.h"

using namespace Clockwork;

namespace ClockworkEditor
{

class ExternalTool : public Object {

    OBJECT(ExternalTool);

public:

    /// Construct.
    ExternalTool(Context* context);
    /// Destruct.
    virtual ~ExternalTool();

    virtual bool Launch(const String& args) = 0;

    virtual void Open(const String& fullpath) {}

protected:

    bool LaunchInternal(const String& applicationPath, const String& args);

    SharedPtr<Subprocess> subprocess_;

private:



};

}
