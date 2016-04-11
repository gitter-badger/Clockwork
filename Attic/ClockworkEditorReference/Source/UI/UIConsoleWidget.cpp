// Copyright (c) 2014-2015, THUNDERBEAST GAMES LLC All rights reserved
// Please see LICENSE.md in repository root for license information
// https://github.com/ClockworkGameEngine/ClockworkGameEngine

#include "ClockworkEditor.h"

#include <TurboBadger/tb_layout.h>

#include <Clockwork/Core/Context.h>
#include <Clockwork/IO/Log.h>
#include <Clockwork/UI/UI.h>

#include "UIListView.h"
#include "UIConsoleWidget.h"
#include "UIMainFrame.h"
#include "UIResourceFrame.h"

#include <Clockwork/IO/FileSystem.h>

#include <ClockworkJS/Javascript/JSEvents.h>

#include "../CEJavascript.h"
#include "../CEEvents.h"

namespace ClockworkEditor
{

ConsoleWidget::ConsoleWidget(Context* context) :
    CEWidget(context),
    consolelayout_(0)
{
    context_->RegisterSubsystem(this);
    SubscribeToEvent(E_EDITORSHUTDOWN, HANDLER(ConsoleWidget, HandleEditorShutdown));

    UI* tbui = GetSubsystem<UI>();
    tbui->LoadResourceFile(delegate_, "ClockworkEditor/editor/ui/consolewidget.tb.txt");

    consolelayout_ = delegate_->GetWidgetByIDAndType<TBLayout>(TBIDC("consolelayout"));
    assert(consolelayout_);

    consoleList_ = new ListView(context_, "consolelist");

    TBWidgetDelegate* consoleListWD = consoleList_->GetWidgetDelegate();

    consoleListWD->SetGravity(WIDGET_GRAVITY_ALL);

    consolelayout_->AddChild(consoleListWD);

    SubscribeToEvent(E_JSPRINT, HANDLER(ConsoleWidget, HandleMessage));
    SubscribeToEvent(E_CONSOLEMESSAGE, HANDLER(ConsoleWidget, HandleMessage));
    SubscribeToEvent(E_CONSOLEERRORMESSAGE, HANDLER(ConsoleWidget, HandleMessage));

}

ConsoleWidget::~ConsoleWidget()
{

}

void ConsoleWidget::HandleMessage(StringHash eventType, VariantMap& eventData)
{
    if (eventType == E_CONSOLEMESSAGE)
    {
        using namespace ConsoleMessage;
        const String& message = eventData[P_TEXT].GetString();
    }
    else if (eventType == E_JSPRINT)
    {
        using namespace JSPrint;
        const String& message = eventData[P_TEXT].GetString();
        consoleList_->AddItem(message.CString(), NULL, TBID(consoleList_->GetNumItems()));
    }
    else if (eventType == E_CONSOLEERRORMESSAGE)
    {
        using namespace ConsoleErrorMessage;
        const String& message = eventData[P_TEXT].GetString();
        consoleList_->AddItem(message.CString(), NULL, TBID(consoleList_->GetNumItems()));
    }
}

bool ConsoleWidget::OnEvent(const TBWidgetEvent &ev)
{

    return false;
}

void ConsoleWidget::HandleEditorShutdown(StringHash eventType, VariantMap& eventData)
{
    context_->RemoveSubsystem(GetType());
}

}

