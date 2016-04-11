// Copyright (c) 2014-2015, THUNDERBEAST GAMES LLC All rights reserved
// Please see LICENSE.md in repository root for license information
// https://github.com/ClockworkGameEngine/ClockworkGameEngine

#pragma once

#include "CEWidget.h"

using namespace Clockwork;
using namespace tb;

// replaces resource frame while playing

namespace ClockworkEditor
{

class PlayerWidget: public CEWidget
{
    OBJECT(PlayerWidget);

public:

    PlayerWidget(Context* context);
    virtual ~PlayerWidget();

    bool OnEvent(const TBWidgetEvent &ev);

};

}
