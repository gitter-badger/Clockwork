#include <rapidjson/document.h>
#include <rapidjson/filestream.h>
#include <rapidjson/prettywriter.h>

#include "ClockworkEditor.h"
#include <Clockwork/Core/Context.h>
#include <Clockwork/IO/Log.h>
#include "AEEditor.h"
#include "AEEvents.h"
#include "AEVersion.h"
#include "AEVersionCheck.h"

namespace ClockworkEditor
{

VersionCheck::VersionCheck(Context* context) :
    Object(context)

{
    SubscribeToEvent(E_EDITORSHUTDOWN, HANDLER(VersionCheck, HandleEditorShutdown));
    progressModal_ = new ProgressModal(context_, "Checking for Updates", "Checking for updates, please wait...");
}

VersionCheck::~VersionCheck()
{

}

void VersionCheck::DoVersionCheck()
{
    progressModal_->Show();

    CurlManager* cm = GetSubsystem<CurlManager>();

    versionRequest_ = cm->MakeRequest("https://store.clockworkgameengine.com/versions/clockwork_editor.html");

    SubscribeToEvent(versionRequest_, E_CURLCOMPLETE, HANDLER(VersionCheck, HandleCurlComplete));

}

void VersionCheck::HandleCurlComplete(StringHash eventType, VariantMap& eventData)
{
    progressModal_->Hide();

    Editor* editor = GetSubsystem<Editor>();

    bool valid = true;
    int major;
    int minor;
    int patch;

    if (versionRequest_->GetError().Length())
    {
        valid = false;
    }
    else
    {
        rapidjson::Document document;
        String json = versionRequest_->GetResponse();

        if (document.Parse<0>(json.CString()).HasParseError())
        {
            valid = false;
        }
        else
        {
            const rapidjson::Value::Member* jmajor = document.FindMember("major");
            if (jmajor && jmajor->value.IsInt())
            {
                major = jmajor->value.GetInt();
            }
            else
                valid = false;

            const rapidjson::Value::Member* jminor = document.FindMember("minor");
            if (jminor && jminor->value.IsInt())
            {
                minor = jminor->value.GetInt();
            }
            else
                valid = false;

            const rapidjson::Value::Member* jpatch = document.FindMember("patch");
            if (jpatch && jpatch->value.IsInt())
            {
                patch = jpatch->value.GetInt();
            }
            else
                valid = false;

        }

    }

    if (!valid)
    {
        String errorMessage;
        errorMessage.AppendWithFormat("There was an error contacting the version server\n\n%s", versionRequest_->GetError().CString());
        editor->PostModalError("Error Contacting Version Server", errorMessage);
    }
    else
    {
        bool newer = false;
        if (CLOCKWORK_EDITOR_VERSION_MAJOR < major)
            newer = true;
        else if (CLOCKWORK_EDITOR_VERSION_MINOR < minor)
            newer = true;
        else if (CLOCKWORK_EDITOR_VERSION_PATCH < patch)
            newer = true;

        if (newer)
        {
            editor->PostModalInfo("New Version Available", "There is a newer version of the Clockwork Editor available");
        }
        else
        {
            editor->PostModalInfo("Up to date", "The Clockwork Editor is up to date");
        }

    }

    versionRequest_ = 0;

}

void VersionCheck::HandleEditorShutdown(StringHash eventType, VariantMap& eventData)
{
    context_->RemoveSubsystem(GetType());
}

/*
{
    "major" : 0,
    "minor" : 2,
    "patch" : 0
}
*/


}

