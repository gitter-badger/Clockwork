#include "ClockworkEditor.h"

#include <TurboBadger/tb_window.h>
#include <TurboBadger/tb_select.h>
#include <TurboBadger/tb_editfield.h>

#include <Clockwork/Core/Context.h>
#include <Clockwork/UI/UI.h>

#include "Resources/AEResourceOps.h"
#include "AEPreferences.h"

#include "AEEditor.h"
#include "AEEvents.h"
#include "Project/AEProject.h"
#include "Project/ProjectUtils.h"
#include "License/AELicenseSystem.h"

#include "UIPlatformsInfo.h"

namespace ClockworkEditor
{

// UIBuildSettings------------------------------------------------

PlatformsInfo::PlatformsInfo(Context* context):
    UIModalOpWindow(context)
{
    Editor* editor = GetSubsystem<Editor>();

    UI* tbui = GetSubsystem<UI>();
    window_->SetSettings(WINDOW_SETTINGS_DEFAULT & ~WINDOW_SETTINGS_CLOSE_BUTTON);
    window_->SetText("Clockwork Game Engine Pro Required");

    tbui->LoadResourceFile(window_->GetContentRoot(), "ClockworkEditor/editor/ui/platformsinfo.tb.txt");

    window_->ResizeToFitContent();
    Center();

}


PlatformsInfo::~PlatformsInfo()
{
}

bool PlatformsInfo::OnEvent(const TBWidgetEvent &ev)
{
    if (ev.type == EVENT_TYPE_CLICK)
    {
        UIModalOps* ops = GetSubsystem<UIModalOps>();
        if (ev.target->GetID() == TBIDC("ok"))
        {
            ops->Hide();
            return true;
        }
        if (ev.target->GetID() == TBIDC("purchase"))
        {

            //Editor* editor = GetSubsystem<Editor>();
            FileSystem* fs = GetSubsystem<FileSystem>();
            fs->SystemOpen("https://store.clockworkgameengine.com/site");
            ops->Hide();

            return true;
        }


    }

    return false;
}

}