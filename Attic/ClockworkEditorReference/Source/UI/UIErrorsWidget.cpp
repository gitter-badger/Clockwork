#include "ClockworkEditor.h"

#include <TurboBadger/tb_layout.h>
#include <Clockwork/Core/Context.h>
#include <Clockwork/IO/Log.h>
#include <Clockwork/IO/FileSystem.h>
#include <Clockwork/UI/UI.h>

#include "AEEvents.h"
#include "UIListView.h"
#include "UIErrorsWidget.h"
#include "UIMainFrame.h"
#include "UIResourceFrame.h"

namespace ClockworkEditor
{

ErrorsWidget::ErrorsWidget(Context* context) :
    AEWidget(context),
    errorlayout_(0)
{
    context_->RegisterSubsystem(this);

    UI* tbui = GetSubsystem<UI>();
    tbui->LoadResourceFile(delegate_, "ClockworkEditor/editor/ui/errorswidget.tb.txt");

    errorlayout_ = delegate_->GetWidgetByIDAndType<TBLayout>(TBIDC("errorlayout"));
    assert(errorlayout_);

    errorList_ = new ListView(context_, "errorlist");

    TBWidgetDelegate* errorListWD = errorList_->GetWidgetDelegate();

    errorListWD->SetGravity(WIDGET_GRAVITY_ALL);

    errorlayout_->AddChild(errorListWD);

    SubscribeToEvent(E_EDITORSHUTDOWN, HANDLER(ErrorsWidget, HandleEditorShutdown));
}

ErrorsWidget::~ErrorsWidget()
{

}

void ErrorsWidget::UpdateErrors()
{
    AEPlayer* player  = GetSubsystem<AEPlayer>();
    if (!player)
        return;

    const Vector<AEPlayerError>& errors = player->GetErrors();
    errors_ = errors;

    errorList_->DeleteAllItems();

    for (unsigned i = 0; i < errors.Size(); i++)
    {
        const AEPlayerError& error = errors[i];

        String filename = GetFileNameAndExtension(error.filename_);

        String errorString;
        errorString.AppendWithFormat("%s - %s - Line: %i",
                                    filename.CString(), error.message_.CString(), error.lineNumber_);

        errorList_->AddItem(errorString.CString(), NULL, TBID(i));
    }

}

bool ErrorsWidget::OnEvent(const TBWidgetEvent &ev)
{
    if (!ev.target)
        return false;

    TBID id = ev.target->GetID();

    if (ev.type == EVENT_TYPE_CLICK)
    {
        // we clicked the folder list
        if (ev.target->GetID() == TBID("errorlist"))
        {
            TBSelectList* list = (TBSelectList*) ev.target;
            TBID tbid = list->GetSelectedItemID();

            if (tbid < errors_.Size())
            {
                const AEPlayerError& error = errors_[tbid];

                // make sure we are editing it
                ResourceFrame* rframe = GetSubsystem<MainFrame>()->GetResourceFrame();
                rframe->EditResource(error.filename_);
                rframe->NavigateToResource(error.filename_, error.lineNumber_, -1);
            }
        }
    }

    return false;
}

void ErrorsWidget::HandleEditorShutdown(StringHash eventType, VariantMap& eventData)
{
    context_->RemoveSubsystem(GetType());
}

}

