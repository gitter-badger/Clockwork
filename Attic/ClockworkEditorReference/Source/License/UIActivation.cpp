// BEGIN LICENSE MANAGEMENT

#include "ClockworkEditor.h"

#include <TurboBadger/tb_window.h>
#include <TurboBadger/tb_select.h>
#include <TurboBadger/tb_editfield.h>

#include <Clockwork/Core/Context.h>
#include <Clockwork/IO/Log.h>
#include <Clockwork/UI/UI.h>

#include "../Resources/AEResourceOps.h"
#include "../AEPreferences.h"

#include "../AEEditor.h"
#include "../AEEvents.h"
#include "../Project/AEProject.h"
#include "../Project/ProjectUtils.h"

#include "UIActivation.h"
#include "AELicenseSystem.h"

namespace ClockworkEditor
{

// UIBuildSettings------------------------------------------------

UIActivation::UIActivation(Context* context):
    UIModalOpWindow(context),
    licenseKey_(0)
{
    UI* tbui = GetSubsystem<UI>();
    window_->SetSettings(WINDOW_SETTINGS_DEFAULT & ~WINDOW_SETTINGS_CLOSE_BUTTON);
    window_->SetText("Product Activation");
    tbui->LoadResourceFile(window_->GetContentRoot(), "ClockworkEditor/editor/ui/activation.tb.txt");

    licenseKey_ = delegate_->GetWidgetByIDAndType<TBEditField>(TBIDC("license_key"));
    assert(licenseKey_);

    window_->ResizeToFitContent();
    Center();

    progressModal_ = new ProgressModal(context_, "Activation", "Activating, please wait...");
}

UIActivation::~UIActivation()
{

}

void UIActivation::RequestServerActivation(const String& key)
{
    if (serverActivation_.NotNull())
    {
        LOGERROR("UIActivation::RequestServerActivation - request already exists");
        return;
    }

    LicenseSystem* licenseSystem = GetSubsystem<LicenseSystem>();

    key_ = key;
    CurlManager* cm = GetSubsystem<CurlManager>();
    String post;
    String id = licenseSystem->GenerateMachineID();
    post.AppendWithFormat("key=%s&id=%s", key.CString(), id.CString());

    // todo, this should be a verify url (shouldn't auto add id)
    serverActivation_ = cm->MakeRequest("https://store.clockworkgameengine.com/licenses/license_activate.php", post);

    SubscribeToEvent(serverActivation_, E_CURLCOMPLETE, HANDLER(UIActivation, HandleCurlComplete));
}


bool UIActivation::OnEvent(const TBWidgetEvent &ev)
{
    Editor* editor = GetSubsystem<Editor>();
    LicenseSystem* licenseSystem = GetSubsystem<LicenseSystem>();

    if (ev.type == EVENT_TYPE_CLICK)
    {
        if (ev.target->GetID() == TBIDC("get_key"))
        {
            FileSystem* fileSystem = GetSubsystem<FileSystem>();
            fileSystem->SystemOpen("https://store.clockworkgameengine.com/site");
        }
        else if (ev.target->GetID() == TBIDC("quit"))
        {
            editor->RequestExit();
        }
        else if (ev.target->GetID() == TBIDC("activate"))
        {
            TBStr _key;
            licenseKey_->GetText(_key);
            String key = _key.CStr();
            key = key.ToUpper().Trimmed();
            if (!licenseSystem->ValidateKey(key))
            {
                editor->PostModalError("Invalid Product Key",
                                       "The key entered is invalid\n\nProduct keys are in the form of CLOCKWORK-XXXX-XXXX-XXXX-XXXX");
            }
            else
            {
                progressModal_->Show();
                RequestServerActivation(key);
            }

            return true;
        }
    }


    return false;
}

void UIActivation::HandleCurlComplete(StringHash eventType, VariantMap& eventData)
{
    // we want to stay alive through the call, though will be swapped for success modal
    SharedPtr<UIModalOpWindow> keepAlive(this);

    Editor* editor = GetSubsystem<Editor>();
    LicenseSystem* licenseSystem = GetSubsystem<LicenseSystem>();


    progressModal_->Hide();

    if (serverActivation_->GetError().Length())
    {
        String errorMessage;
        errorMessage.AppendWithFormat("There was an error contacting the activation server\n\n%s", serverActivation_->GetError().CString());
        editor->PostModalError("Error Contacting Activation Server", errorMessage);
    }
    else
    {
        LicenseSystem::LicenseParse parse;
        int code = licenseSystem->ParseResponse(serverActivation_->GetResponse(), parse);

        if (code == 0)
        {

            licenseSystem->Activate(key_, parse);

            UIModalOps* ops = GetSubsystem<UIModalOps>();
            ops->Hide();
            ops->ShowActivationSuccess();

        }
        else if (code == 1)
        {
            editor->PostModalError("Activations Exceeded",
                            "This key has 2 activations in use.\n\nPlease return a license from Clockwork Editor - Manage License menu on one of these active computers.\n\nIf you are unable to do so, please contact sales@clockworkgameengine.com providing the key to reset it");
        }
        else if (code == 2)
        {
            editor->PostModalError("License Error",
                            "There was a problem with the license key.\n\nPlease check the key and try again.\n\nIf the problem persists please contact sales@clockworkgameengine.com");

        }
        else if (code == 3)
        {
            editor->PostModalError("Activation Server Error",
                            "There was an error on the activation server\n\nIf the problem persists please contact sales@clockworkgameengine.com");

        }

    }

    UnsubscribeFromEvents(serverActivation_);
    serverActivation_ = 0;


}

}

// END LICENSE MANAGEMENT
