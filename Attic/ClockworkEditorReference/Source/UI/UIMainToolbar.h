// Copyright (c) 2014-2015, THUNDERBEAST GAMES LLC All rights reserved
// Please see LICENSE.md in repository root for license information
// https://github.com/ClockworkGameEngine/ClockworkGameEngine

#pragma once

#include "CEWidget.h"

using namespace Clockwork;

namespace ClockworkEditor
{

class MainToolbar : public CEWidget
{
    OBJECT(MainToolbar);

public:
    /// Construct.
    MainToolbar(Context* context);
    /// Destruct.
    virtual ~MainToolbar();

    bool OnEvent(const TBWidgetEvent &ev);

    void Show3DWidgets(bool value);

private:

};

}
