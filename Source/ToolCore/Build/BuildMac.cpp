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

#include <Clockwork/IO/FileSystem.h>
#include <Clockwork/Resource/ResourceCache.h>

#include "../ToolSystem.h"
#include "../ToolEnvironment.h"
#include "../Project/Project.h"

#include "BuildEvents.h"
#include "BuildSystem.h"
#include "BuildMac.h"

namespace ToolCore
{

BuildMac::BuildMac(Context * context, Project *project) : BuildBase(context, project, PLATFORMID_MAC)
{

}

BuildMac::~BuildMac()
{

}

void BuildMac::Initialize()
{
    ToolSystem* tsystem = GetSubsystem<ToolSystem>();

    Project* project = tsystem->GetProject();

    Vector<String> defaultResourcePaths;
    GetDefaultResourcePaths(defaultResourcePaths);
    String projectResources = project->GetResourcePath();

    for (unsigned i = 0; i < defaultResourcePaths.Size(); i++)
    {
        AddResourceDir(defaultResourcePaths[i]);
    }

    // TODO: smart filtering of cache
    AddResourceDir(project->GetProjectPath() + "Cache/");
    AddResourceDir(projectResources);

    BuildResourceEntries();

}

void BuildMac::Build(const String& buildPath)
{
    ToolEnvironment* tenv = GetSubsystem<ToolEnvironment>();

    buildPath_ = AddTrailingSlash(buildPath) + GetBuildSubfolder();

    BuildLog("Starting Mac Deployment");

    Initialize();

    BuildSystem* buildSystem = GetSubsystem<BuildSystem>();

    if (!BuildRemoveDirectory(buildPath_))
        return;

    FileSystem* fileSystem = GetSubsystem<FileSystem>();

    String appSrcPath = tenv->GetPlayerAppFolder();

    if (!BuildCreateDirectory(buildPath_))
        return;

    buildPath_ += "/ClockworkPlayer.app";

    if (!BuildCreateDirectory(buildPath_))
        return;
    if (!BuildCreateDirectory(buildPath_ + "/Contents"))
        return;
    if (!BuildCreateDirectory(buildPath_ + "/Contents/MacOS"))
        return;
    if (!BuildCreateDirectory(buildPath_ + "/Contents/Resources"))
        return;

    String resourcePackagePath = buildPath_ + "/Contents/Resources/ClockworkResources" + PAK_EXTENSION;
    GenerateResourcePackage(resourcePackagePath);

    if (!BuildCopyFile(appSrcPath + "/Contents/Resources/Clockwork.icns", buildPath_ + "/Contents/Resources/Clockwork.icns"))
        return;

    if (!BuildCopyFile(appSrcPath + "/Contents/Info.plist", buildPath_ + "/Contents/Info.plist"))
        return;

    if (!BuildCopyFile(appSrcPath + "/Contents/MacOS/ClockworkPlayer", buildPath_ + "/Contents/MacOS/ClockworkPlayer"))
        return;

#ifdef CLOCKWORK_PLATFORM_OSX
    Vector<String> args;
    args.Push("+x");
    args.Push(buildPath_ + "/Contents/MacOS/ClockworkPlayer");
    fileSystem->SystemRun("chmod", args);
#endif

    BuildLog("Mac Deployment Complete");

    buildPath_ = buildPath + "/Mac-Build";
    buildSystem->BuildComplete(PLATFORMID_MAC, buildPath_);

}

}