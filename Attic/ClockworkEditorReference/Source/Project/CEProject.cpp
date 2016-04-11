// Copyright (c) 2014-2015, THUNDERBEAST GAMES LLC All rights reserved
// Please see LICENSE.md in repository root for license information
// https://github.com/ClockworkGameEngine/ClockworkGameEngine

#include <rapidjson/document.h>
#include <rapidjson/filestream.h>
#include <rapidjson/prettywriter.h>

#include "ClockworkEditor.h"
#include <Clockwork/IO/File.h>
#include <Clockwork/IO/FileSystem.h>
#include <Clockwork/IO/Log.h>
#include <Clockwork/Core/Context.h>

#include "../CEEditor.h"
#include "Build/BuildSystem.h"
#include "CEProject.h"

using namespace rapidjson;

namespace ClockworkEditor
{

Project::Project(Context* context) :
    Object(context),
    loading_(false)
{
	
}

Project::~Project()
{

}

void Project::LoadUserPrefs(const String& fullpath)
{
    rapidjson::Document document;

    File jsonFile(context_, fullpath);

    if (!jsonFile.IsOpen())
        return;

    String json;
    jsonFile.ReadText(json);

    if (!json.Length())
        return;

    if (document.Parse<0>(json.CString()).HasParseError())
    {
        LOGERRORF("Could not parse Project JSON data from %s", fullpath.CString());
        return;
    }

    const Value::Member* current_platform = document.FindMember("current_platform");

    CEEditorPlatform platform = CE_PLATFORM_UNDEFINED;
    if (current_platform && current_platform->value.IsString())
    {
        String splatform = current_platform->value.GetString();
        if (splatform == "Windows")
            platform = CE_PLATFORM_WINDOWS;
        else if (splatform == "Mac")
            platform = CE_PLATFORM_MAC;
        else if (splatform == "HTML5")
            platform = CE_PLATFORM_HTML5;
        else if (splatform == "iOS")
            platform = CE_PLATFORM_IOS;
        else if (splatform == "Android")
            platform = CE_PLATFORM_ANDROID;
    }
    if (platform == CE_PLATFORM_UNDEFINED)
    {
#ifdef CLOCKWORK_PLATFORM_OSX
        platform = CE_PLATFORM_MAC;
#else
        platform = CE_PLATFORM_WINDOWS;
#endif
    }

    const Value::Member* last_build_path = document.FindMember("last_build_path");
    if (last_build_path && last_build_path->value.IsString())
    {
        lastBuildPath_ = last_build_path->value.GetString();
    }

    // probably will want to move this, it will trigger a save (which is guarded with load_)
    Editor* editor = GetSubsystem<Editor>();
    editor->RequestPlatformChange(platform);

}

void Project::SaveUserPrefs(const String& fullpath)
{
    Editor* editor = GetSubsystem<Editor>();
    FILE* file = fopen(fullpath.CString(), "w");

    if (!file)
        return;

    rapidjson::FileStream s(file);
    rapidjson::PrettyWriter<rapidjson::FileStream> writer(s);

    writer.StartObject();
    writer.String("version");
    writer.Int(1);

    writer.String("current_platform");
    CEEditorPlatform platform = editor->GetCurrentPlatform();
    if (platform == CE_PLATFORM_WINDOWS)
        writer.String("Windows");
    else if (platform == CE_PLATFORM_MAC)
        writer.String("Mac");
    else if (platform == CE_PLATFORM_HTML5)
        writer.String("HTML5");
    else if (platform == CE_PLATFORM_IOS)
        writer.String("iOS");
    else if (platform == CE_PLATFORM_ANDROID)
        writer.String("Android");

    writer.String("last_build_path");
    writer.String(lastBuildPath_.CString());

    writer.EndObject();

    fclose(file);
}

void Project::Load(const String& fullpath)
{
    loading_ = true;
    projectFilePath_ = fullpath;

    LoadUserPrefs(GetUserPrefsFullPath(fullpath));

    rapidjson::Document document;

    File jsonFile(context_, fullpath);

    if (!jsonFile.IsOpen())
    {
        loading_ = false;
        return;
    }

    String json;
    jsonFile.ReadText(json);

    if (!json.Length())
    {
        loading_ = false;
        return;
    }

    if (document.Parse<0>(json.CString()).HasParseError())
    {
        LOGERRORF("Could not parse Project JSON data from %s", fullpath.CString());
        loading_ = false;
        return;
    }

    const Value::Member* version = document.FindMember("version");
    if (version && version->value.IsInt())
    {

    }

    Value::Member* build_settings = document.FindMember("build_settings");
    if (build_settings && build_settings->value.IsObject())
    {
        BuildSystem* buildSystem = GetSubsystem<BuildSystem>();
        buildSystem->LoadBuildSettings(build_settings);
    }

    loading_ = false;
}

String Project::GetUserPrefsFullPath(const String& projectPath)
{
    String path = GetPath(projectPath);
    String filename = GetFileName(projectPath);
    String prefsPath = path + "/" + filename + ".clockwork.userprefs";
    return prefsPath;
}

void Project::Save(const String& fullpath)
{
    if (loading_)
        return;

    if (fullpath.Length())
        projectFilePath_ = fullpath;

    String path = projectFilePath_;

    SaveUserPrefs(GetUserPrefsFullPath(path));

    FILE* file = fopen(path.CString(), "w");

    if (!file)
        return;

    rapidjson::FileStream s(file);
    rapidjson::PrettyWriter<rapidjson::FileStream> writer(s);

    writer.StartObject();
    writer.String("version");
    writer.Int(1);

    BuildSystem* buildSystem = GetSubsystem<BuildSystem>();
    buildSystem->SaveBuildSettings(writer);

    writer.EndObject();

    fclose(file);

}

bool Project::IsComponentsDirOrFile(const String& fullPath)
{
    String pathName;
    String fileName;
    String extension;

    SplitPath(fullPath, pathName, fileName, extension);

    if (extension.Length() && extension != ".js")
        return false;

    if (IsAbsoluteParentPath(componentsPath_, pathName))
        return true;

    return false;

}

bool Project::IsScriptsDirOrFile(const String& fullPath)
{
    String pathName;
    String fileName;
    String extension;

    SplitPath(fullPath, pathName, fileName, extension);

    if (extension.Length() && extension != ".js")
        return false;

    if (IsAbsoluteParentPath(scriptsPath_, pathName))
        return true;

    return false;

}

bool Project::IsModulesDirOrFile(const String& fullPath)
{
    String pathName;
    String fileName;
    String extension;

    SplitPath(fullPath, pathName, fileName, extension);

    if (extension.Length() && extension != ".js")
        return false;

    if (IsAbsoluteParentPath(modulesPath_, pathName))
        return true;

    return false;

}


}
