// BEGIN LICENSE MANAGEMENT

#pragma once

#include "UI/Modal/UIModalOps.h"
#include "UI/Modal/UIProgressModal.h"
#include "Net/CurlManager.h"

#include <TurboBadger/tb_select.h>
#include <TurboBadger/tb_select_item.h>

namespace ClockworkEditor
{

class UIActivation: public UIModalOpWindow
{
    OBJECT(UIActivation);

public:

    UIActivation(Context* context);
    virtual ~UIActivation();
    bool OnEvent(const TBWidgetEvent &ev);

private:

    void RequestServerActivation(const String& key);

    void HandleCurlComplete(StringHash eventType, VariantMap& eventData);

    TBEditField* licenseKey_;
    String key_;

    SharedPtr<CurlRequest> serverActivation_;

    SharedPtr<ProgressModal> progressModal_;


};

}

// END LICENSE MANAGEMENT
