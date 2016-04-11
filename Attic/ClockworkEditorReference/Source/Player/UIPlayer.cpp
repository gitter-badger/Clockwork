// Copyright (c) 2014-2015, THUNDERBEAST GAMES LLC All rights reserved
// Please see LICENSE.md in repository root for license information
// https://github.com/ClockworkGameEngine/ClockworkGameEngine

#include "ClockworkEditor.h"

#include <TurboBadger/tb_window.h>
#include <TurboBadger/tb_select.h>
#include <TurboBadger/tb_editfield.h>

#include <Clockwork/Core/Context.h>
#include <Clockwork/Core/CoreEvents.h>
#include <Clockwork/IO/Log.h>
#include <Clockwork/UI/UI.h>

#include <Clockwork/Graphics/Graphics.h>
#include <Clockwork/Graphics/Viewport.h>
#include <Clockwork/Graphics/Camera.h>

#include "UI/Modal/UIModalOps.h"
#include "License/CELicenseSystem.h"

#include "CEEditor.h"
#include "CEEvents.h"

#include "UIPlayer.h"

namespace Clockwork
{
    void FixMeSetLight2DGroupViewport(Viewport *viewport);
}

namespace ClockworkEditor
{

// UIBuildSettings------------------------------------------------

UIPlayer::UIPlayer(Context* context):
    UIModalOpWindow(context)
{

    aePlayer_ = GetSubsystem<CEPlayer>();
    aePlayer_->SetUIPlayer(this);

    UI* tbui = GetSubsystem<UI>();

    window_->SetSettings(WINDOW_SETTINGS_DEFAULT & ~WINDOW_SETTINGS_CLOSE_BUTTON);

    window_->SetText("Clockwork Player");
    tbui->LoadResourceFile(window_->GetContentRoot(), "ClockworkEditor/editor/ui/playerwidget.tb.txt");

    Graphics* graphics = GetSubsystem<Graphics>();

    float gwidth = graphics->GetWidth();
    float aspect = float(graphics->GetHeight())/ gwidth;
    gwidth -= 150;

    playerSize_.x_ = gwidth;
    playerSize_.y_ = gwidth * aspect;

    TBLayout* playercontainer = window_->GetWidgetByIDAndType<TBLayout>(TBIDC("playerlayout"));
    assert(playercontainer);

    window_->ResizeToFitContent();

    Center();    

    SubscribeToEvent(E_UPDATE, HANDLER(UIPlayer, HandleUpdate));

}


UIPlayer::~UIPlayer()
{

}

void UIPlayer::HandleUpdate(StringHash eventType, VariantMap& eventData)
{

}


bool UIPlayer::OnEvent(const TBWidgetEvent &ev)
{
    if (ev.type == EVENT_TYPE_CLICK)
    {
        if (ev.target->GetID() == TBIDC("play"))
        {
            if (GetSubsystem<Editor>()->IsPlayingProject())
            {
                SendEvent(E_EDITORPLAYSTOP);                               
                return true;
            }
        }
    }

    return true;
}



}


