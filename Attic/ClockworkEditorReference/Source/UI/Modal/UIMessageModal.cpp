#include "ClockworkEditor.h"

#include <Clockwork/Core/Context.h>
#include "AEEvents.h"

#include "UI/UIMainFrame.h"
#include "UI/Modal/UIMessageModal.h"

#include <TurboBadger/tb_message_window.h>

namespace ClockworkEditor
{

/// Construct.
MessageModal::MessageModal(Context* context) :
    Object(context)
{
    SubscribeToEvent(E_EDITORMODAL, HANDLER(MessageModal, HandleEditorModal));
}

/// Destruct.
MessageModal::~MessageModal()
{

}

void MessageModal::ShowErrorWindow(const String& title, const String& message)
{
    MainFrame* mainframe = GetSubsystem<MainFrame>();
    TBMessageWindow *msg_win = new TBMessageWindow(mainframe->GetWidgetDelegate(), TBIDC("modal_error"));
    TBMessageWindowSettings settings(TB_MSG_OK, TBID(uint32(0)));
    settings.dimmer = true;
    settings.styling = true;
    msg_win->Show(title.CString(), message.CString(), &settings, 640, 360);
}

void MessageModal::ShowInfoWindow(const String& title, const String& message)
{
    MainFrame* mainframe = GetSubsystem<MainFrame>();
    TBMessageWindow *msg_win = new TBMessageWindow(mainframe->GetWidgetDelegate(), TBIDC("modal_info"));
    TBMessageWindowSettings settings(TB_MSG_OK, TBID(uint32(0)));
    settings.dimmer = true;
    settings.styling = true;
    msg_win->Show(title.CString(), message.CString(), &settings, 640, 360);
}


void MessageModal::HandleEditorModal(StringHash eventType, VariantMap& eventData)
{
    using namespace EditorModal;

    const String& title = eventData[P_TITLE].GetString();
    const String& message = eventData[P_MESSAGE].GetString();

    if (eventData[P_TYPE].GetUInt() == EDITOR_MODALERROR)
        ShowErrorWindow(title, message);

    if (eventData[P_TYPE].GetUInt() == EDITOR_MODALINFO)
        ShowInfoWindow(title, message);

}


}
