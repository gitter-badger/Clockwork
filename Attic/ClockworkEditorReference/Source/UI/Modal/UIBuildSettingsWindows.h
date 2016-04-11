// Copyright (c) 2014-2015, THUNDERBEAST GAMES LLC All rights reserved
// Please see LICENSE.md in repository root for license information
// https://github.com/ClockworkGameEngine/ClockworkGameEngine

#pragma once

#include "UI/CEWidget.h"

using namespace Clockwork;
using namespace tb;

namespace tb
{
class TBLayout;
}

namespace ClockworkEditor
{

class UIBuildSettingsWindows: public CEWidget
{
    OBJECT(UIBuildSettingsWindows);

public:

    UIBuildSettingsWindows(Context* context);
    virtual ~UIBuildSettingsWindows();

    bool OnEvent(const TBWidgetEvent &ev);

    void Refresh();
    void StoreSettings();

    //void HandleMessage(StringHash eventType, VariantMap& eventData);

private:

    TBEditField* appNameEdit_;
    TBEditField* appPackageEdit_;
    TBEditField* productNameEdit_;
    TBEditField* companyNameEdit_;

};

}
