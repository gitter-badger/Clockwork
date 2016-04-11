// Copyright (c) 2014-2015, THUNDERBEAST GAMES LLC All rights reserved
// Please see LICENSE.md in repository root for license information
// https://github.com/ClockworkGameEngine/ClockworkGameEngine

#include "ClockworkEditor.h"

#include <Clockwork/UI/UI.h>
#include <Clockwork/IO/Log.h>

#include "UIPlayerWidget.h"

using namespace tb;

namespace ClockworkEditor
{

PlayerWidget::PlayerWidget(Context* context) :
    CEWidget(context)
{
    UI* tbui = GetSubsystem<UI>();
    tbui->LoadResourceFile(delegate_, "ClockworkEditor/editor/ui/playerwidget.tb.txt");
}

PlayerWidget::~PlayerWidget()
{

}

bool PlayerWidget::OnEvent(const TBWidgetEvent &ev)
{
    return false;
}

}
