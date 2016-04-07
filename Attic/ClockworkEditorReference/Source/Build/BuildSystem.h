#pragma once

#include <Clockwork/Core/Object.h>
#include "BuildSettings.h"
#include "BuildBase.h"

using namespace Clockwork;

#include "UI/Modal/UIBuildComplete.h"

namespace ClockworkEditor
{

class BuildSystem : public Object
{
    OBJECT(BuildSystem);

public:
    /// Construct.
    BuildSystem(Context* context);
    /// Destruct.
    virtual ~BuildSystem();

    BuildSettings* GetBuildSettings() { return buildSettings_; }

    void LoadBuildSettings(rapidjson::Value::Member* jobject);
    void SaveBuildSettings(rapidjson::PrettyWriter<rapidjson::FileStream>& writer);

    void ClearBuildCompleteUI();
    void BuildComplete(AEEditorPlatform platform,const String& buildFolder, bool success = true, bool fail3D = false);

private:

    void DoBuildMac(const String& buildPath);
    void DoBuildWindows(const String& buildPath);
    void DoBuildAndroid(const String& buildPath);
    void DoBuildIOS(const String& buildPath);
    void DoBuildWeb(const String& buildPath);

    SharedPtr<BuildSettings> buildSettings_;
    SharedPtr<BuildBase> currentBuild_;

    SharedPtr<UIBuildComplete> uiBuildComplete_;

    void HandleEditorBuild(StringHash eventType, VariantMap& eventData);
    void HandleEditorShutdown(StringHash eventType, VariantMap& eventData);
};


}
