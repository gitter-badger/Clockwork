#include "ClockworkEditor.h"

#include <TurboBadger/tb_layout.h>
#include <TurboBadger/tb_editfield.h>

#include <Clockwork/Core/Context.h>
#include <Clockwork/IO/Log.h>
#include <Clockwork/UI/UI.h>

#include "Build/BuildSystem.h"
#include "UIBuildSettingsMac.h"

namespace ClockworkEditor
{

UIBuildSettingsMac::UIBuildSettingsMac(Context* context) :
    AEWidget(context)
{
    UI* tbui = GetSubsystem<UI>();
    tbui->LoadResourceFile(delegate_, "ClockworkEditor/editor/ui/buildsettings_mac.tb.txt");

    appNameEdit_ = delegate_->GetWidgetByIDAndType<TBEditField>(TBIDC("app_name"));
    assert(appNameEdit_);

    appPackageEdit_ = delegate_->GetWidgetByIDAndType<TBEditField>(TBIDC("app_identifier"));
    assert(appPackageEdit_);

    productNameEdit_ = delegate_->GetWidgetByIDAndType<TBEditField>(TBIDC("product_name"));
    assert(productNameEdit_);

    companyNameEdit_ = delegate_->GetWidgetByIDAndType<TBEditField>(TBIDC("company_name"));
    assert(companyNameEdit_);

}

UIBuildSettingsMac::~UIBuildSettingsMac()
{

}

void UIBuildSettingsMac::Refresh()
{
    BuildSystem* buildSystem = GetSubsystem<BuildSystem>();

    const MacBuildSettings& settings = buildSystem->GetBuildSettings()->GetMacSettings();

    appNameEdit_->SetText(settings.appName.CString());
    appPackageEdit_->SetText(settings.package.CString());
    productNameEdit_->SetText(settings.productName.CString());
    companyNameEdit_->SetText(settings.companyName.CString());

}

void UIBuildSettingsMac::StoreSettings()
{
    BuildSystem* buildSystem = GetSubsystem<BuildSystem>();

    MacBuildSettings settings;

    TBStr text;
    appNameEdit_->GetText(text);
    settings.appName = text.CStr();
    text.Clear();

    appPackageEdit_->GetText(text);
    settings.package = text.CStr();
    text.Clear();

    productNameEdit_->GetText(text);
    settings.productName = text.CStr();
    text.Clear();

    companyNameEdit_->GetText(text);
    settings.companyName = text.CStr();
    text.Clear();


    buildSystem->GetBuildSettings()->SetMacSettings(settings);

}



bool UIBuildSettingsMac::OnEvent(const TBWidgetEvent &ev)
{
    return false;
}



}

