// Copyright (c) 2014-2015, THUNDERBEAST GAMES LLC All rights reserved
// Please see LICENSE.md in repository root for license information
// https://github.com/ClockworkGameEngine/ClockworkGameEngine

#pragma once

#include "CEWidget.h"

using namespace Clockwork;
using namespace tb;

namespace tb
{
class TBLayout;
}

namespace ClockworkEditor
{

class ListView;

class ConsoleWidget: public CEWidget
{
    OBJECT(ConsoleWidget);

public:

    ConsoleWidget(Context* context);
    virtual ~ConsoleWidget();

    bool OnEvent(const TBWidgetEvent &ev);

    void HandleMessage(StringHash eventType, VariantMap& eventData);

private:

    void HandleEditorShutdown(StringHash eventType, VariantMap& eventData);

    SharedPtr<ListView> consoleList_;
    TBLayout* consolelayout_;

};

}
