// Copyright (c) 2014-2015, THUNDERBEAST GAMES LLC All rights reserved
// Please see LICENSE.md in repository root for license information
// https://github.com/ClockworkGameEngine/ClockworkGameEngine

#pragma once

#include "BuildBase.h"

namespace ClockworkEditor
{

class BuildMac : public BuildBase
{
    OBJECT(BuildMac);

public:

    BuildMac(Context* context);
    virtual ~BuildMac();

    void Build(const String& buildPath);

protected:

    void Initialize();


};

}
