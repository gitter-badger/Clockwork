// Copyright (c) 2014-2015, THUNDERBEAST GAMES LLC All rights reserved
// Please see LICENSE.md in repository root for license information
// https://github.com/ClockworkGameEngine/ClockworkGameEngine

#include "ClockworkEditor.h"

#include <rapidjson/document.h>
#include "rapidjson/prettywriter.h"
#include "rapidjson/filestream.h"

#include <Clockwork/Core/Context.h>
#include <Clockwork/IO/FileSystem.h>
#include <Clockwork/IO/Log.h>
#include <Clockwork/IO/File.h>
#include <Clockwork/Graphics/Graphics.h>

#include "CEEvents.h"
#include "CEPreferences.h"

using namespace rapidjson;

namespace ClockworkEditor
{

CEPreferences::CEPreferences(Context* context) :
    Object(context)
{
    context->RegisterSubsystem(this);

    SubscribeToEvent(E_EDITORSHUTDOWN, HANDLER(CEPreferences, HandleEditorShutdown));

    Read();
}

CEPreferences::~CEPreferences()
{

}

String CEPreferences::GetPreferencesFullPath()
{
    FileSystem* fs = GetSubsystem<FileSystem>();
    String filepath = fs->GetAppPreferencesDir("ClockworkEditor", "Preferences");
    filepath += "prefs.json";
    return filepath;
}

void CEPreferences::Clear()
{
    recentProjects_.Clear();
}

void CEPreferences::Read()
{
    rapidjson::Document document;

    String filepath = GetPreferencesFullPath();

    File jsonFile(context_, filepath);

    if (!jsonFile.IsOpen())
        return;

    String json;
    jsonFile.ReadText(json);

    if (!json.Length())
        return;

    if (document.Parse<0>(json.CString()).HasParseError())
    {
        LOGERRORF("Could not parse JSON data from %s", filepath.CString());
        return;
    }

    Clear();

    const Value::Member* recent_files = document.FindMember("recent_files");
    if (recent_files && recent_files->value.IsArray())
    {
        for (Value::ConstValueIterator itr = recent_files->value.Begin(); itr != recent_files->value.End(); itr++)
        {
             if (!(*itr).IsString())
                 continue;

            String path(itr->GetString());
            recentProjects_.Push(path.CString());
        }
    }

    const Value::Member* android_sdk_path = document.FindMember("android_sdk_path");
    if (android_sdk_path && android_sdk_path->value.IsString())
        androidSDKPath_ = android_sdk_path->value.GetString();

    const Value::Member* jdk_root_path = document.FindMember("jdk_root_path");
    if (jdk_root_path && jdk_root_path->value.IsString())
        jdkRootPath_ = jdk_root_path->value.GetString();

    const Value::Member* ant_path = document.FindMember("ant_path");
    if (ant_path && ant_path->value.IsString())
        antPath_ = ant_path->value.GetString();

    UpdateRecentFiles(false);

}

void CEPreferences::Write()
{
    String filepath = GetPreferencesFullPath();

    FILE* file = fopen(filepath.CString(), "w");

    if (!file)
        return;

    Graphics* graphics = GetSubsystem<Graphics>();

    IntVector2 pos(-1, -1);

    int width = -1;
    int height = -1;

    if (graphics && !graphics->GetFullscreen())
    {
        pos = graphics->GetWindowPosition();
        width = graphics->GetWidth();
        height = graphics->GetHeight();
    }

    rapidjson::FileStream s(file);
    rapidjson::PrettyWriter<rapidjson::FileStream> writer(s);

    writer.StartObject();

    // recent files
    writer.String("recent_files");
    writer.StartArray();
    for (unsigned i = 0; i < recentProjects_.Size(); i++)
        writer.String(recentProjects_[i].CString());
    writer.EndArray();

    writer.String("android_sdk_path");
    writer.String(androidSDKPath_.CString());

    writer.String("jdk_root_path");
    writer.String(jdkRootPath_.CString());

    writer.String("ant_path");
    writer.String(antPath_.CString());

    writer.String("window_pos_x");
    writer.Int(pos.x_);
    writer.String("window_pos_y");
    writer.Int(pos.y_);
    writer.String("window_width");
    writer.Int(width);
    writer.String("window_height");
    writer.Int(height);

    writer.EndObject();

    fclose(file);

}

void CEPreferences::UpdateRecentFiles(bool write)
{
    FileSystem* fileSystem = GetSubsystem<FileSystem>();
    Vector<String> recentProjects;

    for (unsigned i = 0; i < recentProjects_.Size(); i++)
    {
        String path = recentProjects_[i];

        if (!fileSystem->FileExists(path))
            continue;


        recentProjects.Push(path);

        if (recentProjects.Size() > 0 && recentProjects.Size() <= 15)
            break;
    }

    recentProjects_ = recentProjects;

    if (write)
        Write();
}

void CEPreferences::RegisterRecentProject(const String& fullpath)
{
    if (recentProjects_.Contains(fullpath))
    {
        recentProjects_.Remove(fullpath);
    }

    recentProjects_.Insert(0, fullpath);


    UpdateRecentFiles();

}

bool CEPreferences::ReadStartupPrefs(Context *context, StartupPreferences& prefs)
{

    FileSystem* fileSystem = context->GetSubsystem<FileSystem>();
    String filepath = fileSystem->GetAppPreferencesDir("ClockworkEditor", "Preferences");
    filepath += "prefs.json";

    if (!fileSystem->FileExists(filepath))
        return false;

    SharedPtr<File> file(new File(context, filepath, FILE_READ));

    if (!file->IsOpen())
        return false;

    String json;
    file->ReadText(json);

    if (!json.Length())
        return false;

    rapidjson::Document document;

    if (document.Parse<0>(json.CString()).HasParseError())
    {
        return false;
    }

    bool success = true;

    const Value::Member* imember = document.FindMember("window_pos_x");
    if (imember && imember->value.IsInt())
    {
        prefs.windowPos.x_ = imember->value.GetInt();
    }
    else
    {
        success = false;
    }

    imember = document.FindMember("window_pos_y");
    if (imember && imember->value.IsInt())
    {
        prefs.windowPos.y_ = imember->value.GetInt();
    }
    else
    {
        success = false;
    }

    imember = document.FindMember("window_width");
    if (imember && imember->value.IsInt())
    {
        prefs.windowWidth = imember->value.GetInt();
    }
    else
    {
        success = false;
    }

    imember = document.FindMember("window_height");
    if (imember && imember->value.IsInt())
    {
        prefs.windowHeight = imember->value.GetInt();
    }
    else
    {
        success = false;
    }

    if (prefs.windowHeight < 128 || prefs.windowWidth < 128)
        return false;

    return success;

}

void CEPreferences::HandleEditorShutdown(StringHash eventType, VariantMap& eventData)
{
    context_->RemoveSubsystem(GetType());
}

}
