// Copyright (c) 2014-2015, THUNDERBEAST GAMES LLC All rights reserved
// Please see LICENSE.md in repository root for license information
// https://github.com/ClockworkGameEngine/ClockworkGameEngine

#include "ClockworkEditor.h"
#include <Clockwork/IO/FileSystem.h>
#include <Clockwork/IO/File.h>
#include "../CEEditor.h"
#include "../Project/CEProject.h"

#include "../Project/ProjectUtils.h"

#include "BuildSystem.h"
#include "BuildWeb.h"

namespace ClockworkEditor
{

BuildWeb::BuildWeb(Context * context) : BuildBase(context)
{

}

BuildWeb::~BuildWeb()
{

}

void BuildWeb::Initialize()
{
    Editor* editor = GetSubsystem<Editor>();
    Project* project = editor->GetProject();

    FileSystem* fileSystem = GetSubsystem<FileSystem>();
    String bundleResources = fileSystem->GetAppBundleResourceFolder();

    String projectResources = project->GetResourcePath();
    String coreDataFolder = bundleResources + "CoreData/";

    AddResourceDir(coreDataFolder);
    AddResourceDir(projectResources);

    BuildResourceEntries();
}
void BuildWeb::Build(const String& buildPath)
{
    buildPath_ = buildPath + "/Web-Build";

    Initialize();

    FileSystem* fileSystem = GetSubsystem<FileSystem>();
    if (fileSystem->DirExists(buildPath_))
        fileSystem->RemoveDir(buildPath_, true);

    String buildSourceDir = fileSystem->GetAppBundleResourceFolder();
    buildSourceDir += "Deployment/Web";

    fileSystem->CreateDir(buildPath_);

    String resourcePackagePath = buildPath_ + "/ClockworkResources.data";
    GenerateResourcePackage(resourcePackagePath);

    fileSystem->Copy(buildSourceDir + "/ClockworkPlayer.html", buildPath_ + "/ClockworkPlayer.html");
    fileSystem->Copy(buildSourceDir + "/ClockworkPlayer.js", buildPath_ + "/ClockworkPlayer.js");

    File file(context_, buildSourceDir + "/ClockworkResources_js.template", FILE_READ);
    unsigned size = file.GetSize();

    SharedArrayPtr<char> buffer(new char[size + 1]);
    file.Read(buffer.Get(), size);
    buffer[size] = '\0';

    String resourcejs = (const char*) buffer.Get();

    file.Close();

    file.Open(buildPath_ + "/ClockworkResources.data", FILE_READ);
    int rsize = (int) file.GetSize();
    file.Close();

    String request;
    request.AppendWithFormat("new DataRequest(0, %i, 0, 0).open('GET', '/ClockworkResources.pak');", rsize);

    resourcejs.Replace("$$CLOCKWORK_RESOURCES_DATA_REQUEST$$", request);

    file.Open(buildPath_ + "/ClockworkResources.js", FILE_WRITE);
    file.Write(resourcejs.CString(), resourcejs.Length());
    file.Close();

    BuildSystem* buildSystem = GetSubsystem<BuildSystem>();
    buildSystem->BuildComplete(CE_PLATFORM_HTML5, buildPath_);

    //fileSystem->SystemCommandAsync("/Applications/Firefox.app/Contents/MacOS/firefox");

}

}
