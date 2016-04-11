//
// Copyright (c) 2014-2016 THUNDERBEAST GAMES LLC
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

// before resource system exists so use rapidjson directly
#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/filestream.h>

#include <Clockwork/IO/Log.h>
#include <Clockwork/IO/FileSystem.h>
#include <Clockwork/IO/File.h>

#include "ToolEnvironment.h"

using namespace rapidjson;


namespace ToolCore
{

ToolEnvironment::ToolEnvironment(Context* context) : Object(context),
    toolPrefs_(new ToolPrefs(context))
{

}

ToolEnvironment::~ToolEnvironment()
{

}

bool ToolEnvironment::InitFromPackage()
{
    toolPrefs_->Load();

    FileSystem* fileSystem = GetSubsystem<FileSystem>();

#ifdef CLOCKWORK_PLATFORM_WINDOWS
	editorBinary_ = fileSystem->GetProgramDir() + "ClockworkEditor.exe";
    String resourcesDir = fileSystem->GetProgramDir() + "Resources/";
#elif CLOCKWORK_PLATFORM_LINUX
    editorBinary_ = fileSystem->GetProgramDir() + "ClockworkEditor";
    String resourcesDir = fileSystem->GetProgramDir() + "Resources/";
#else
    editorBinary_ = fileSystem->GetProgramDir() + "ClockworkEditor";
    String resourcesDir = GetPath(RemoveTrailingSlash(fileSystem->GetProgramDir())) + "Resources/";
#endif

    //TODO: move this to deployment stuff
    playerAppFolder_ = resourcesDir + "ToolData/Deployment/MacOS/ClockworkPlayer.app/";
    playerBinary_ = resourcesDir + "ToolData/Deployment/Windows/x64/ClockworkPlayer.exe";

    resourceCoreDataDir_ = resourcesDir + "CoreData";
    resourcePlayerDataDir_ = resourcesDir + "PlayerData";

    toolDataDir_ =  resourcesDir + "ToolData/";

    // ClockworkNET
    netAssemblyLoadPaths_ = GetNativePath(ToString("%sClockworkNET/Windows/Clockwork/", resourcesDir.CString()));

#ifdef CLOCKWORK_PLATFORM_WINDOWS
    netCoreCLRAbsPath_ = GetNativePath(ToString("%sClockworkNET/Windows/x64/", resourcesDir.CString()));
    netTPAPaths_ = ToString("%sClockworkNET/Windows/Clockwork/TPA/", resourcesDir.CString());
#else
    netCoreCLRAbsPath_ = GetNativePath(ToString("%sClockworkNET/Windows/x64/", resourcesDir.CString()));
    netTPAPaths_ = ToString("%sClockworkNET/Windows/Clockwork/TPA/", resourcesDir.CString());
#endif

    return true;
}

bool ToolEnvironment::InitFromJSON(bool clockworkTool)
{

    toolPrefs_->Load();

    // make sure config path is initialized
    GetDevConfigFilename();

    FileSystem* fileSystem = GetSubsystem<FileSystem>();

    if (clockworkTool || !fileSystem->FileExists(devConfigFilename_))
    {
        // default to build directories

        SetRootSourceDir(CLOCKWORK_ROOT_SOURCE_DIR);
        SetRootBuildDir(CLOCKWORK_ROOT_BUILD_DIR, true);

        netAssemblyLoadPaths_ = GetNativePath(ToString("%s/Artifacts/ClockworkNET/", CLOCKWORK_ROOT_SOURCE_DIR));
        netClockworkNETEngineAssemblyPath_ = ToString("%s/Artifacts/ClockworkNET/", CLOCKWORK_ROOT_SOURCE_DIR);

#ifdef CLOCKWORK_PLATFORM_WINDOWS
        netCoreCLRAbsPath_ = GetNativePath(ToString("%s/Submodules/CoreCLR/Windows/Release/x64/", CLOCKWORK_ROOT_SOURCE_DIR));
#elif CLOCKWORK_PLATFORM_LINUX
        netCoreCLRAbsPath_ = GetNativePath(ToString("%s/Submodules/CoreCLR/Linux/Debug/x64/", CLOCKWORK_ROOT_SOURCE_DIR));
#else
        netCoreCLRAbsPath_ = GetNativePath(ToString("%s/Submodules/CoreCLR/MacOSX/Debug/x64/", CLOCKWORK_ROOT_SOURCE_DIR));
#endif

        netTPAPaths_ = ToString("%s/Artifacts/ClockworkNET/TPA/", CLOCKWORK_ROOT_SOURCE_DIR);

        return true;
    }

    File jsonFile(context_, devConfigFilename_);

    if (!jsonFile.IsOpen())
        return false;

    String json;
    jsonFile.ReadText(json);

    if (!json.Length())
        return false;

    rapidjson::Document document;
    if (document.Parse<0>(json.CString()).HasParseError())
    {
        return false;
    }

    const Value::Member* rootSourceDir = document.FindMember("rootSourceDir");
    if (rootSourceDir && rootSourceDir->value.IsString())
        SetRootSourceDir(rootSourceDir->value.GetString());
    else
        return false;

    const Value::Member* rootBuildDir = document.FindMember("rootBuildDir");
    if (rootBuildDir && rootBuildDir->value.IsString())
        SetRootBuildDir(rootBuildDir->value.GetString(), true);
    else
        return false;


    return true;

}


const String& ToolEnvironment::GetDevConfigFilename()
{
    if (devConfigFilename_.Length())
        return devConfigFilename_;

    FileSystem* fileSystem = GetSubsystem<FileSystem>();

#ifdef CLOCKWORK_PLATFORM_OSX
    devConfigFilename_ = fileSystem->GetUserDocumentsDir() + ".clockworkgameengine/toolEnv.json";
#elif CLOCKWORK_PLATFORM_WINDOWS
    devConfigFilename_ = fileSystem->GetUserDocumentsDir() + "ClockworkGameEngine/toolEnv.json";
#else
    devConfigFilename_ = fileSystem->GetUserDocumentsDir() + ".clockworkgameengine/toolEnv.json";
#endif

    return devConfigFilename_;
}

void ToolEnvironment::SetRootSourceDir(const String& sourceDir)
{
    rootSourceDir_ = AddTrailingSlash(sourceDir);
    resourceCoreDataDir_ = rootSourceDir_ + "Resources/CoreData";
    resourcePlayerDataDir_ = rootSourceDir_ + "Resources/PlayerData";
    resourceEditorDataDir_ = rootSourceDir_ + "Resources/EditorData";

    toolDataDir_ = rootSourceDir_ + "Data/ClockworkEditor/";
}

void ToolEnvironment::SetRootBuildDir(const String& buildDir, bool setBinaryPaths)
{
    FileSystem* fileSystem = GetSubsystem<FileSystem>();
    rootBuildDir_ = AddTrailingSlash(buildDir);


    if (setBinaryPaths)
    {
#ifdef CLOCKWORK_PLATFORM_WINDOWS

#ifdef _DEBUG
        playerBinary_ = rootBuildDir_ + "Source/ClockworkPlayer/Application/Debug/ClockworkPlayer.exe";
        editorBinary_ = rootBuildDir_ + "Source/ClockworkEditor/Debug/ClockworkEditor.exe";
#else
        playerBinary_ = rootBuildDir_ + "Source/ClockworkPlayer/Application/Release/ClockworkPlayer.exe";
        editorBinary_ = rootBuildDir_ + "Source/ClockworkEditor/Release/ClockworkEditor.exe";
#endif

        // some build tools like ninja don't use Release/Debug folders
        if (!fileSystem->FileExists(playerBinary_))
                playerBinary_ = rootBuildDir_ + "Source/ClockworkPlayer/Application/ClockworkPlayer.exe";
        if (!fileSystem->FileExists(editorBinary_))
                editorBinary_ = rootBuildDir_ + "Source/ClockworkEditor/ClockworkEditor.exe";

        playerAppFolder_ = rootSourceDir_ + "Data/ClockworkEditor/Deployment/MacOS/ClockworkPlayer.app";

#elif CLOCKWORK_PLATFORM_OSX

#ifdef CLOCKWORK_XCODE
        playerBinary_ = rootBuildDir_ + "Source/ClockworkPlayer/" + CMAKE_INTDIR + "/ClockworkPlayer.app/Contents/MacOS/ClockworkPlayer";
        editorBinary_ = rootBuildDir_ + "Source/ClockworkEditor/" + CMAKE_INTDIR + "/ClockworkEditor.app/Contents/MacOS/ClockworkEditor";
#else
        playerBinary_ = rootBuildDir_ + "Source/ClockworkPlayer/Application/ClockworkPlayer.app/Contents/MacOS/ClockworkPlayer";
        playerAppFolder_ = rootBuildDir_ + "Source/ClockworkPlayer/Application/ClockworkPlayer.app/";
        editorBinary_ = rootBuildDir_ + "Source/ClockworkEditor/ClockworkEditor.app/Contents/MacOS/ClockworkEditor";
#endif

#else
        playerBinary_ = rootBuildDir_ + "Source/ClockworkPlayer/Application/ClockworkPlayer";
        editorBinary_ = rootBuildDir_ + "Source/ClockworkEditor/ClockworkEditor";

#endif
    }

}

String ToolEnvironment::GetIOSDeployBinary()
{
    return GetToolDataDir() + "Deployment/IOS/ios-deploy/ios-deploy";
}

void ToolEnvironment::Dump()
{
    LOGINFOF("Root Source Dir: %s", rootSourceDir_.CString());
    LOGINFOF("Root Build Dir: %s", rootBuildDir_.CString());

    LOGINFOF("Core Resource Dir: %s", resourceCoreDataDir_.CString());
    LOGINFOF("Player Resource Dir: %s", resourcePlayerDataDir_.CString());
    LOGINFOF("Editor Resource Dir: %s", resourceEditorDataDir_.CString());

    LOGINFOF("Editor Binary: %s", editorBinary_.CString());
    LOGINFOF("Player Binary: %s", playerBinary_.CString());
    LOGINFOF("Tool Binary: %s", toolBinary_.CString());


    LOGINFOF("Tool Data Dir: %s", toolDataDir_.CString());

    LOGINFOF("Deployment Data Dir: %s", deploymentDataDir_.CString());

    LOGINFOF("Dev Config File: %s", devConfigFilename_.CString());

}

}
