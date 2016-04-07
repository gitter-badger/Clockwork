#pragma once

#include <rapidjson/document.h>
#include <rapidjson/filestream.h>
#include <rapidjson/prettywriter.h>

#include <Clockwork/Container/Str.h>
#include <Clockwork/Core/Object.h>

using namespace Clockwork;

namespace ClockworkEditor
{

struct AndroidBuildSettings
{
    String appName;
    String package;
    String targetSDKVersion;
    String minSDKVersion;
    String activityName;
    String companyName;
    String productName;
};

struct IOSBuildSettings
{
    String appName;
    String package;
    String companyName;
    String productName;
    String provisionFile;
    String appidPrefix;
};

struct MacBuildSettings
{
    String appName;
    String package;
    String companyName;
    String productName;
};

struct WindowsBuildSettings
{
    String appName;
    String package;
    String companyName;
    String productName;
};

struct WebGLSettings
{
    String appName;
    String package;
    String companyName;
    String productName;
};


class BuildSettings : public Object
{
    OBJECT(BuildSettings);

public:
    /// Construct.
    BuildSettings(Context* context);
    /// Destruct.
    virtual ~BuildSettings();

    const AndroidBuildSettings& GetAndroidSettings() { return android_; }
    void SetAndroidSettings(const AndroidBuildSettings& settings) { android_ = settings; }

    const IOSBuildSettings& GetIOSSettings() { return ios_; }
    void SetIOSSettings(const IOSBuildSettings& settings) { ios_ = settings; }

    const MacBuildSettings& GetMacSettings() { return mac_; }
    void SetMacSettings(const MacBuildSettings& settings) { mac_ = settings; }

    const WindowsBuildSettings& GetWindowsSettings() { return windows_; }
    void SetWindowsSettings(const WindowsBuildSettings& settings) { windows_ = settings; }

    const WebGLSettings& GetWebGLSettings() { return webgl_; }
    void SetWebGLSettings(const WebGLSettings& settings) { webgl_ = settings; }

    void Load(rapidjson::Value::Member* jobject);
    void Save(rapidjson::PrettyWriter<rapidjson::FileStream>& writer);

private:

    String GetStringMember(rapidjson::Value::Member* jobject, const String& name);

    AndroidBuildSettings android_;
    IOSBuildSettings ios_;
    MacBuildSettings mac_;
    WindowsBuildSettings windows_;
    WebGLSettings webgl_;

};

}
