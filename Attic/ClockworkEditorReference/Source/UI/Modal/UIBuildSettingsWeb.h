#pragma once

#include "UI/AEWidget.h"

using namespace Clockwork;
using namespace tb;

namespace tb
{
class TBLayout;
class TBEditField;
class TBTextField;
class TBSelectDropdown;
}

namespace ClockworkEditor
{

class UIBuildSettingsWeb: public AEWidget
{
    OBJECT(UIBuildSettingsWeb);

public:

    UIBuildSettingsWeb(Context* context);
    virtual ~UIBuildSettingsWeb();

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
