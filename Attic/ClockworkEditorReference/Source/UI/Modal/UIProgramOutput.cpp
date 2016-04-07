#include <TurboBadger/tb_window.h>
#include <TurboBadger/tb_editfield.h>

#include "ClockworkEditor.h"
#include <Clockwork/Core/Context.h>
#include <Clockwork/UI/UI.h>

#include "AEEvents.h"
#include "UIProgramOutput.h"

namespace ClockworkEditor
{

// UINewFolder------------------------------------------------

UIProgramOutput::UIProgramOutput(Context* context):
    UIModalOpWindow(context)
{

    UI* tbui = GetSubsystem<UI>();
    window_->SetText("Program Output");
    tbui->LoadResourceFile(window_->GetContentRoot(), "ClockworkEditor/editor/ui/programoutput.tb.txt");
    output_ = window_->GetContentRoot()->GetWidgetByIDAndType<TBEditField>(TBIDC("output"));
    assert(output_);
    window_->ResizeToFitContent();
    Center();
}

UIProgramOutput::~UIProgramOutput()
{

}

void UIProgramOutput::HandleEvent(StringHash eventType, VariantMap& eventData)
{
    if (eventType == E_SUBPROCESSOUTPUT)
    {
        String text = eventData[SubprocessOutput::P_TEXT].GetString();
        TBStr tbText;
        output_->GetText(tbText);
        tbText.Append(text.CString());
        output_->SetText(tbText);
        output_->ScrollTo(0, 0xffffff);
    }
    else if (eventType == E_SUBPROCESSCOMPLETE)
    {

    }
}

void UIProgramOutput::OutputText(const String& text)
{
    TBStr tbText;
    output_->GetText(tbText);
    tbText.Append(text.CString());
    output_->SetText(tbText);
    output_->ScrollTo(0, 0xffffff);
}

void UIProgramOutput::SetSubprocess(Object* subprocess)
{
    UnsubscribeFromAllEvents();

    SubscribeToEvent(subprocess, E_SUBPROCESSOUTPUT, HANDLER(UIProgramOutput, HandleEvent));
    SubscribeToEvent(subprocess, E_SUBPROCESSCOMPLETE, HANDLER(UIProgramOutput, HandleEvent));

}


bool UIProgramOutput::OnEvent(const TBWidgetEvent &ev)
{
    UIModalOps* ops = GetSubsystem<UIModalOps>();

    if (ev.type == EVENT_TYPE_CLICK)
    {
        if (ev.target->GetID() == TBIDC("create"))
        {
            ops->Hide();
            return true;
        }

        if (ev.target->GetID() == TBIDC("ok"))
        {
            ops->Hide();
            return true;
        }

        if (ev.target->GetID() == TBIDC("cancel"))
        {
            ops->Hide();
            return true;
        }
    }

    return false;
}

}
