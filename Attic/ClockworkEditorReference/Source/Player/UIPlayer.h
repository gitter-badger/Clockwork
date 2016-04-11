// Copyright (c) 2014-2015, THUNDERBEAST GAMES LLC All rights reserved
// Please see LICENSE.md in repository root for license information
// https://github.com/ClockworkGameEngine/ClockworkGameEngine


#pragma once

#include "UI/Modal/UIModalOps.h"
#include "UI/UIView3D.h"

#include "CEPlayer.h"

namespace Clockwork
{
class Scene;
class Camera;
}

namespace ClockworkEditor
{

/// Player modal

class UIPlayer: public UIModalOpWindow
{
    OBJECT(UIPlayer);

public:

    UIPlayer(Context* context);
    virtual ~UIPlayer();

    bool OnEvent(const TBWidgetEvent &ev);
    const IntVector2& GetPlayerSize() { return playerSize_; }

private:

    void HandleUpdate(StringHash eventType, VariantMap& eventData);
    WeakPtr<CEPlayer> aePlayer_;
    IntVector2 playerSize_;
};

}
