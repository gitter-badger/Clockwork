// Copyright (c) 2014-2015, THUNDERBEAST GAMES LLC All rights reserved
// Please see LICENSE.md in repository root for license information
// https://github.com/ClockworkGameEngine/ClockworkGameEngine

// BEGIN LICENSE MANAGEMENT

#include "ClockworkEditor.h"

#include <TurboBadger/tb_window.h>
#include <TurboBadger/tb_select.h>
#include <TurboBadger/tb_editfield.h>
#include <TurboBadger/tb_tab_container.h>

#include <Clockwork/Core/Context.h>
#include <Clockwork/IO/File.h>
#include <Clockwork/Resource/ResourceCache.h>
#include <Clockwork/Input/InputEvents.h>
#include <Clockwork/UI/UI.h>

#include "Resources/CEResourceOps.h"
#include "CEPreferences.h"

#include "CEEditor.h"
#include "CEEvents.h"
#include "Project/CEProject.h"
#include "Project/ProjectUtils.h"

#include "License/CELicenseSystem.h"
#include "UIEulaAgreement.h"

namespace ClockworkEditor
{

// UIBuildSettings------------------------------------------------

UIEulaAgreement::UIEulaAgreement(Context* context):
    UIModalOpWindow(context)
{
    UI* tbui = GetSubsystem<UI>();
    window_->SetSettings(WINDOW_SETTINGS_DEFAULT & ~WINDOW_SETTINGS_CLOSE_BUTTON);
    window_->SetText("License Agreement");
    tbui->LoadResourceFile(window_->GetContentRoot(), "ClockworkEditor/editor/ui/eulaagreement.tb.txt");

    eulaCheck_ = window_->GetWidgetByIDAndType<TBCheckBox>(TBIDC("eula_check"));
    assert(eulaCheck_);

    TBEditField* age_license = window_->GetWidgetByIDAndType<TBEditField>(TBIDC("age_license"));
    assert(age_license);

    TBEditField* thirdparty_license = window_->GetWidgetByIDAndType<TBEditField>(TBIDC("thirdparty_license"));
    assert(thirdparty_license);

    TBEditField* externaltool_license = window_->GetWidgetByIDAndType<TBEditField>(TBIDC("externaltool_license"));
    assert(externaltool_license);


    ResourceCache* cache = GetSubsystem<ResourceCache>();

    SharedPtr<File> file = cache->GetFile("ClockworkEditor/eulas/clockwork_game_engine_eula.txt");
    String text;
    file->ReadText(text);
    age_license->SetText(text.CString());

    file = cache->GetFile("ClockworkEditor/eulas/clockwork_thirdparty_eula.txt");
    file->ReadText(text);
    thirdparty_license->SetText(text.CString());

    file = cache->GetFile("ClockworkEditor/eulas/clockwork_external_tools_eula.txt");
    file->ReadText(text);
    externaltool_license->SetText(text.CString());

    window_->ResizeToFitContent();
    Center();

    TBTabContainer* container = window_->GetWidgetByIDAndType<TBTabContainer>(TBIDC("tabcontainer"));
    assert(container);
    container->SetValue(0);

}


UIEulaAgreement::~UIEulaAgreement()
{
}

bool UIEulaAgreement::OnEvent(const TBWidgetEvent &ev)
{
    if (ev.type == EVENT_TYPE_CLICK)
    {
        if (ev.target->GetID() == TBIDC("quit"))
        {
            SendEvent(E_EXITREQUESTED);
            return true;
        }
        else if (ev.target->GetID() == TBIDC("ok"))
        {
            Editor* editor = GetSubsystem<Editor>();

            if (!eulaCheck_->GetValue())
            {
                editor->PostModalInfo("License Agreement", "Please agree to licensing terms and conditions to continue");
                return true;
            }

            SharedPtr<UIEulaAgreement> keepAlive(this);
            GetSubsystem<UIModalOps>()->Hide();
            LicenseSystem* licenseSystem = GetSubsystem<LicenseSystem>();
            licenseSystem->LicenseAgreementConfirmed();
            return true;
        }


    }


    return false;
}

}

// END LICENSE MANAGEMENT
