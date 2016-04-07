#include "ClockworkEditor.h"

#include <Clockwork/IO/Log.h>
#include <Clockwork/UI/UI.h>

#include <TurboBadger/tb_widgets.h>
#include <TurboBadger/tb_window.h>
#include <TurboBadger/tb_menu_window.h>

#include "UIMainFrame.h"

using namespace tb;


namespace ClockworkEditor
{

TBWidgetDelegate::TBWidgetDelegate(AEWidget *aewidget)
{
    aewidget_ = aewidget;
}

bool TBWidgetDelegate::OnEvent(const TBWidgetEvent &ev)
{
    return aewidget_->OnEvent(ev);
}


AEWidget::AEWidget(Context* context) :
    Object(context)
{
    delegate_ = new TBWidgetDelegate(this);
}

AEWidget::~AEWidget()
{
    if (delegate_ && delegate_->GetParent())
    {
        delegate_->GetParent()->RemoveChild(delegate_);
        delete delegate_;
    }
}

}
