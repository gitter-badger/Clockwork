// Copyright (c) 2014-2015, THUNDERBEAST GAMES LLC All rights reserved
// Please see LICENSE.md in repository root for license information
// https://github.com/ClockworkGameEngine/ClockworkGameEngine

// BEGIN LICENSE MANAGEMENT

#pragma once

#include "UI/Modal/UIModalOps.h"

#include <TurboBadger/tb_widgets_common.h>

namespace ClockworkEditor
{

class UIEulaAgreement: public UIModalOpWindow
{
    OBJECT(UIEulaAgreement);

public:

    UIEulaAgreement(Context* context);
    virtual ~UIEulaAgreement();
    bool OnEvent(const TBWidgetEvent &ev);

private:

    TBCheckBox* eulaCheck_;

};

}

// END LICENSE MANAGEMENT
