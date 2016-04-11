// Copyright (c) 2014-2015, THUNDERBEAST GAMES LLC All rights reserved
// Please see LICENSE.md in repository root for license information
// https://github.com/ClockworkGameEngine/ClockworkGameEngine

#include "ClockworkEditor.h"

#include <TurboBadger/tb_window.h>
#include <TurboBadger/tb_select.h>
#include <TurboBadger/tb_editfield.h>

#include <Clockwork/Core/Context.h>
#include <Clockwork/UI/UI.h>

#include "Resources/CEResourceOps.h"
#include "CEPreferences.h"

#include "CEEditor.h"
#include "CEEvents.h"
#include "Project/CEProject.h"
#include "Project/ProjectUtils.h"

#include "UIBuild.h"

namespace ClockworkEditor
{

// UIBuildSettings------------------------------------------------

UIBuild::UIBuild(Context* context):
    UIModalOpWindow(context)
  , buildPathField_(0)
{
    Editor* editor = GetSubsystem<Editor>();
    Project* project = editor->GetProject();

    UI* tbui = GetSubsystem<UI>();
    window_->SetText("Build Player");
    tbui->LoadResourceFile(window_->GetContentRoot(), "ClockworkEditor/editor/ui/build.tb.txt");

    buildPathField_ = delegate_->GetWidgetByIDAndType<TBTextField>(TBIDC("build_path"));
    assert(buildPathField_);

    String buildPath = project->GetLastBuildPath();
    buildPathField_->SetText(buildPath.CString());

    window_->ResizeToFitContent();
    Center();

    CEEditorPlatform platform = editor->GetCurrentPlatform();
    TBSkinImage* platformIndicator = window_->GetContentRoot()->GetWidgetByIDAndType<TBSkinImage>(TBIDC("current_platform_indicator"));
    assert(platformIndicator);

    if (platform == CE_PLATFORM_MAC)
        platformIndicator->SetSkinBg(TBIDC("LogoMac"));
    else if (platform == CE_PLATFORM_WINDOWS)
        platformIndicator->SetSkinBg(TBIDC("LogoWindows"));
    else if (platform == CE_PLATFORM_ANDROID)
        platformIndicator->SetSkinBg(TBIDC("LogoAndroid"));
    else if (platform == CE_PLATFORM_HTML5)
        platformIndicator->SetSkinBg(TBIDC("LogoHTML5"));
    else if (platform == CE_PLATFORM_IOS)
        platformIndicator->SetSkinBg(TBIDC("LogoIOS"));

}


UIBuild::~UIBuild()
{
}

bool UIBuild::OnEvent(const TBWidgetEvent &ev)
{
    UIModalOps* ops = GetSubsystem<UIModalOps>();        
    ProjectUtils* utils = context_->GetSubsystem<ProjectUtils>();

    if (ev.type == EVENT_TYPE_CLICK)
    {

        if (ev.target->GetID() == TBIDC("choose_path"))
        {

            String buildPath = utils->GetBuildPath("");

            if (buildPath.Length())
                buildPathField_->SetText(buildPath.CString());

            return true;
        }
        else if (ev.target->GetID() == TBIDC("build"))
        {
            // we're going to be hiding this modal
            // and chaining the program output modal
            // potentially, but we want this modal to
            // survive, this needs to be refactored
            // until the end of this call
            SharedPtr<UIModalOpWindow> keepAlive(this);

            TBStr str;
            buildPathField_->GetText(str);
            String buildPath = str.CStr();
            if (buildPath.Length())
            {
                Editor* editor = GetSubsystem<Editor>();
                Project* project = editor->GetProject();
                project->SetLastBuildPath(buildPath);
                project->Save();

                using namespace EditorBuild;

                VariantMap neventData;

                CEEditorPlatform platform = editor->GetCurrentPlatform();
                if (platform == CE_PLATFORM_ANDROID)
                    neventData[P_PLATFORM] = "Android";
                else if (platform == CE_PLATFORM_WINDOWS)
                    neventData[P_PLATFORM] = "Windows";
                else if (platform == CE_PLATFORM_MAC)
                    neventData[P_PLATFORM] = "Mac";
                else if (platform == CE_PLATFORM_HTML5)
                    neventData[P_PLATFORM] = "HTML5";
                else if (platform == CE_PLATFORM_IOS)
                    neventData[P_PLATFORM] = "IOS";

                ops->Hide();
                neventData[P_BUILDPATH] = buildPath;
                SendEvent(E_EDITORBUILD, neventData);
            }


            return true;
        }
        else if (ev.target->GetID() == TBIDC("cancel"))
        {
            ops->Hide();
            return true;
        }
    }

    return false;
}

}
