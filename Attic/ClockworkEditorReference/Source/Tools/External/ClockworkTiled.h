// Copyright (c) 2014-2015, THUNDERBEAST GAMES LLC All rights reserved
// Please see LICENSE.md in repository root for license information
// https://github.com/ClockworkGameEngine/ClockworkGameEngine

#pragma once

#include "CEExternalTool.h"

using namespace Clockwork;

namespace ClockworkEditor
{

class ClockworkTiled : public ExternalTool
{

    OBJECT(ClockworkTiled);

public:

    /// Construct.
    ClockworkTiled(Context* context);
    /// Destruct.
    virtual ~ClockworkTiled();

    void Open(const String& fullpath);

    bool Launch(const String& args);

protected:

    void HandleEvent(StringHash eventType, VariantMap& eventData);

};

}
