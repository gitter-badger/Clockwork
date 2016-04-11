// Copyright (c) 2014-2015, THUNDERBEAST GAMES LLC All rights reserved
// Please see LICENSE.md in repository root for license information
// https://github.com/ClockworkGameEngine/ClockworkGameEngine

// BEGIN LICENSE MANAGEMENT

#include "ClockworkEditor.h"

#include <TurboBadger/tb_window.h>
#include <TurboBadger/tb_select.h>
#include <TurboBadger/tb_editfield.h>
#include <TurboBadger/tb_message_window.h>

#include <Clockwork/Core/Context.h>
#include <Clockwork/UI/UI.h>

#include "Resources/CEResourceOps.h"
#include "CEPreferences.h"

#include "CEEditor.h"
#include "CEEvents.h"
#include "Project/CEProject.h"
#include "Project/ProjectUtils.h"

#include "CELicenseSystem.h"
#include "UIManageLicense.h"

namespace ClockworkEditor
{

// UIBuildSettings------------------------------------------------

UIManageLicense::UIManageLicense(Context* context):
    UIModalOpWindow(context)

{
    Editor* editor = GetSubsystem<Editor>();
    Project* project = editor->GetProject();

    UI* tbui = GetSubsystem<UI>();
    window_->SetText("Manage License");
    tbui->LoadResourceFile(window_->GetContentRoot(), "ClockworkEditor/editor/ui/managelicense.tb.txt");

    window_->ResizeToFitContent();
    Center();

    progressModal_ = new ProgressModal(context_, "Manage License", "");
}


UIManageLicense::~UIManageLicense()
{
    progressModal_->Hide();
}

bool UIManageLicense::OnEvent(const TBWidgetEvent &ev)
{
    Editor* editor = GetSubsystem<Editor>();
    LicenseSystem* licenseSystem = GetSubsystem<LicenseSystem>();

    if (ev.type == EVENT_TYPE_CLICK)
    {

        if (ev.target->GetID() == TBIDC("ok"))
        {
            UIModalOps* ops = GetSubsystem<UIModalOps>();
            ops->Hide();
            return true;
        }

        if (ev.target->GetID() == TBIDC("confirm_license_return"))
        {
            if (ev.ref_id == TBIDC("TBMessageWindow.ok"))
            {
                request_ = licenseSystem->Deactivate();
                SubscribeToEvent(request_, E_CURLCOMPLETE, HANDLER(UIManageLicense, HandleCurlComplete));
                progressModal_->SetMessage("Returning license, please wait...");
                progressModal_->Show();
            }
        }

        if (ev.target->GetID() == TBIDC("return_activation"))
        {
            if (editor->IsProjectLoaded())
            {
                editor->PostModalError("Close Project", "Please close the current project before deactivating license");
            }
            else
            {
                TBMessageWindow *msg_win = new TBMessageWindow(window_, TBIDC("confirm_license_return"));
                TBMessageWindowSettings settings(TB_MSG_OK_CANCEL, TBID(uint32(0)));
                settings.dimmer = true;
                settings.styling = true;
                msg_win->Show("Return License", "Are you sure you want to return the installed license?", &settings, 300, 140);
            }

            return true;
        }
    }

    return false;
}

void UIManageLicense::HandleCurlComplete(StringHash eventType, VariantMap& eventData)
{
    progressModal_->Hide();
}

}

// END LICENSE MANAGEMENT
