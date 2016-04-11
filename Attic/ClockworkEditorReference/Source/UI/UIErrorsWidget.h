// Copyright (c) 2014-2015, THUNDERBEAST GAMES LLC All rights reserved
// Please see LICENSE.md in repository root for license information
// https://github.com/ClockworkGameEngine/ClockworkGameEngine

#pragma once

#include "CEWidget.h"
#include "../Player/CEPlayer.h"

using namespace Clockwork;
using namespace tb;

namespace tb
{
class TBLayout;
}

namespace ClockworkEditor
{

class ListView;

class ErrorsWidget: public CEWidget
{
    OBJECT(ErrorsWidget);

public:

    ErrorsWidget(Context* context);
    virtual ~ErrorsWidget();

    void UpdateErrors();

    bool OnEvent(const TBWidgetEvent &ev);

private:

    void HandleEditorShutdown(StringHash eventType, VariantMap& eventData);

    Vector<CEPlayerError> errors_;
    SharedPtr<ListView> errorList_;
    TBLayout* errorlayout_;

};

}
