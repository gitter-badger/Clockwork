// Copyright (c) 2014-2015, THUNDERBEAST GAMES LLC All rights reserved
// Please see LICENSE.md in repository root for license information
// https://github.com/ClockworkGameEngine/ClockworkGameEngine

// BEGIN LICENSE MANAGEMENT

#include "ClockworkEditor.h"

#include <TurboBadger/tb_window.h>
#include <TurboBadger/tb_select.h>
#include <TurboBadger/tb_editfield.h>

#include <Clockwork/Core/Context.h>
#include <Clockwork/UI/UI.h>

#include "../Resources/CEResourceOps.h"
#include "../CEPreferences.h"

#include "../CEEditor.h"
#include "../CEEvents.h"
#include "../Project/CEProject.h"
#include "../Project/ProjectUtils.h"


#include "UIActivationSuccess.h"
#include "CELicenseSystem.h"

namespace ClockworkEditor
{

// UIBuildSettings------------------------------------------------

UIActivationSuccess::UIActivationSuccess(Context* context):
    UIModalOpWindow(context)
{
    UI* tbui = GetSubsystem<UI>();
    window_->SetText("Product Activation Successful");
    tbui->LoadResourceFile(window_->GetContentRoot(), "ClockworkEditor/editor/ui/activationsuccess.tb.txt");

    window_->ResizeToFitContent();
    Center();
}

UIActivationSuccess::~UIActivationSuccess()
{

}

bool UIActivationSuccess::OnEvent(const TBWidgetEvent &ev)
{
    UIModalOps* ops = GetSubsystem<UIModalOps>();

    if (ev.type == EVENT_TYPE_CLICK)
    {
        if (ev.target->GetID() == TBIDC("ok"))
        {

            /*
            LicenseSystem* licenseSystem = GetSubsystem<LicenseSystem>();

            if (licenseSystem->IsStarterLicense())
            {
                SharedPtr<UIActivationSuccess> keepAlive(this);
                UIModalOps* ops = GetSubsystem<UIModalOps>();
                ops->Hide();
                ops->ShowPlatformsInfo();
                return true;
            }
            */

            ops->Hide();
            return true;
        }
    }

    return false;
}


}

// END LICENSE MANAGEMENT
