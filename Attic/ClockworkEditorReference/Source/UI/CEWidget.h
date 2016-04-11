// Copyright (c) 2014-2015, THUNDERBEAST GAMES LLC All rights reserved
// Please see LICENSE.md in repository root for license information
// https://github.com/ClockworkGameEngine/ClockworkGameEngine

#pragma once

#include <Clockwork/Core/Object.h>
#include <TurboBadger/tb_widgets.h>
#include <TurboBadger/tb_layout.h>

using namespace Clockwork;
using namespace tb;

namespace ClockworkEditor
{

class TBWidgetDelegate;

class CEWidget : public Object
{
    OBJECT(CEWidget);

protected:

    TBWidgetDelegate* delegate_;

public:

    /// Construct.
    CEWidget(Context* context);
    /// Destruct.
    ~CEWidget();

    TBWidgetDelegate* GetWidgetDelegate() { return delegate_; }

    virtual bool OnEvent(const TBWidgetEvent &ev) { return false; }

};

class TBWidgetDelegate : public tb::TBWidget
{
    CEWidget* aewidget_;

public:

    TBWidgetDelegate(CEWidget *aewidget);

    bool OnEvent(const TBWidgetEvent &ev);
};

}
