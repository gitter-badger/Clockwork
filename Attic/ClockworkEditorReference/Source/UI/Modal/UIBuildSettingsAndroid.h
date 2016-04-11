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
class TBEditField;
class TBSelectDropdown;
}

namespace ClockworkEditor
{

class UIBuildSettingsAndroid: public CEWidget
{
    OBJECT(UIBuildSettingsAndroid);

public:

    UIBuildSettingsAndroid(Context* context);
    virtual ~UIBuildSettingsAndroid();

    bool OnEvent(const TBWidgetEvent &ev);

    void Refresh();

    void StoreSettings();

    //void HandleMessage(StringHash eventType, VariantMap& eventData);

private:

    void HandleEvent(StringHash eventType, VariantMap& eventData);

    void RefreshAndroidTargets();

    String targetOutput_;
    HashMap<unsigned, String> targetLookup_;

    TBEditField* appNameEdit_;
    TBEditField* appPackageEdit_;
    TBEditField* productNameEdit_;
    TBEditField* companyNameEdit_;
    TBSelectDropdown* sdkTargetSelect_;

};

}
