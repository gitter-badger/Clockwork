#pragma once

#include <Clockwork/Core/Object.h>
#include <TurboBadger/tb_widgets.h>
#include <TurboBadger/tb_layout.h>

using namespace Clockwork;
using namespace tb;

namespace ClockworkEditor
{

class TBWidgetDelegate;

class AEWidget : public Object
{
    OBJECT(AEWidget);

protected:

    TBWidgetDelegate* delegate_;

public:

    /// Construct.
    AEWidget(Context* context);
    /// Destruct.
    ~AEWidget();

    TBWidgetDelegate* GetWidgetDelegate() { return delegate_; }

    virtual bool OnEvent(const TBWidgetEvent &ev) { return false; }

};

class TBWidgetDelegate : public tb::TBWidget
{
    AEWidget* aewidget_;

public:

    TBWidgetDelegate(AEWidget *aewidget);

    bool OnEvent(const TBWidgetEvent &ev);
};

}
