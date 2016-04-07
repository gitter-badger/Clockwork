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

#include "Resources/AEResourceOps.h"
#include "AEPreferences.h"

#include "AEVersion.h"
#include "AEEditor.h"
#include "AEEvents.h"
#include "Project/AEProject.h"
#include "Project/ProjectUtils.h"

#include "License/AELicenseSystem.h"
#include "UIAbout.h"

namespace ClockworkEditor
{

UIAbout::UIAbout(Context* context):
    UIModalOpWindow(context)
{
    UI* tbui = GetSubsystem<UI>();
    window_->SetText("About the Clockwork Game Engine");
    tbui->LoadResourceFile(window_->GetContentRoot(), "ClockworkEditor/editor/ui/about.tb.txt");

    TBEditField* age_license = window_->GetWidgetByIDAndType<TBEditField>(TBIDC("age_license"));
    assert(age_license);

    TBEditField* thirdparty_license = window_->GetWidgetByIDAndType<TBEditField>(TBIDC("thirdparty_license"));
    assert(thirdparty_license);

    TBEditField* externaltool_license = window_->GetWidgetByIDAndType<TBEditField>(TBIDC("externaltool_license"));
    assert(externaltool_license);

    TBEditField* about_text = window_->GetWidgetByIDAndType<TBEditField>(TBIDC("about_text"));
    assert(about_text);

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

    GenerateAboutText(text);

    about_text->SetText(text.CString());

    window_->ResizeToFitContent();
    Center();

    TBTabContainer* container = window_->GetWidgetByIDAndType<TBTabContainer>(TBIDC("tabcontainer"));
    assert(container);
    container->SetValue(0);
}


UIAbout::~UIAbout()
{

}

void UIAbout::GenerateAboutText(String& text)
{
    LicenseSystem* licenseSystem = GetSubsystem<LicenseSystem>();
    text.Clear();

    text += "<widget TBImageWidget: filename: 'ClockworkEditor/editor/images/clockwork_logo.png'>\n\n";
    text.AppendWithFormat("<color #D4FB79>Version  %i.%i.p%i</color>\n\n", CLOCKWORK_EDITOR_VERSION_MAJOR, CLOCKWORK_EDITOR_VERSION_MINOR, CLOCKWORK_EDITOR_VERSION_PATCH);
    text += "(c) 2015-2016 CLOCKWORK STUDIOS\n\n\n";

    text += "<color #D4FB79>Installed platforms and modules:</color>\n\n";

    if (licenseSystem->IsStandardLicense())
    {
        text += "    <widget TBSkinImage: skin: 'LogoMac-Small'> <widget TBSkinImage: skin: 'LogoWindows-Small'> <widget TBSkinImage: skin: 'Module2D-Small'>\n\n\n";

        text += "<color #76D6FF>Available platforms and modules:</color>\n\n" \
        "    <widget TBSkinImage: skin: 'LogoHTML5-Small'> <widget TBSkinImage: skin: 'LogoAndroid-Small'> " \
        "<widget TBSkinImage: skin: 'LogoIOS-Small'> <widget TBSkinImage: skin: 'Module3D-Small'> "\
        "<widget TBButton: text: 'Get Pro' skin: 'TBButton.greentext' id: 'purchase_pro' >\n\n\n";
    }
    else
    {
        text += "    <widget TBSkinImage: skin: 'LogoMac-Small'> <widget TBSkinImage: skin: 'LogoWindows-Small'> " \
        "<widget TBSkinImage: skin: 'LogoHTML5-Small'> <widget TBSkinImage: skin: 'LogoAndroid-Small'> " \
        "<widget TBSkinImage: skin: 'LogoIOS-Small'> <widget TBSkinImage: skin: 'Module2D-Small'> <widget TBSkinImage: skin: 'Module3D-Small'>\n\n";
    }

    text += "<color #76D6FF>Special Thanks:</color>\n\n";
    text += "    The Urho3D Project - http://urho3d.github.io\n\n";
    text += "    Sami Vaarala - http://www.duktape.org";
}

bool UIAbout::OnEvent(const TBWidgetEvent &ev)
{
    if (ev.type == EVENT_TYPE_CLICK)
    {
        if (ev.target->GetID() == TBIDC("purchase_pro"))
        {
            FileSystem* fileSystem = GetSubsystem<FileSystem>();
            fileSystem->SystemOpen("https://store.clockworkgameengine.com/site");
        }
        else if (ev.target->GetID() == TBIDC("ok"))
        {
            GetSubsystem<UIModalOps>()->Hide();
            return true;
        }

    }

    return false;
}

}


