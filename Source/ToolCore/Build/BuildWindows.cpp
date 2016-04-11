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

#include <Clockwork/Core/StringUtils.h>
#include <Clockwork/IO/FileSystem.h>
#include <Clockwork/Resource/ResourceCache.h>

#include "../ToolSystem.h"
#include "../ToolEnvironment.h"
#include "../Project/Project.h"

#include "BuildWindows.h"
#include "BuildSystem.h"
#include "BuildEvents.h"

namespace ToolCore
{

BuildWindows::BuildWindows(Context * context, Project *project) : BuildBase(context, project, PLATFORMID_WINDOWS)
{

}

BuildWindows::~BuildWindows()
{

}

void BuildWindows::Initialize()
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

void BuildWindows::BuildClockworkNET()
{
    // ClockworkNET

    FileSystem* fileSystem = GetSubsystem<FileSystem>();
    ToolEnvironment* tenv = GetSubsystem<ToolEnvironment>();
    ToolSystem* tsystem = GetSubsystem<ToolSystem>();
    Project* project = tsystem->GetProject();
    String projectResources = project->GetResourcePath();

    String assembliesPath = projectResources + "Assemblies/";

    // if no assemblies path, no need to install ClockworkNET
    if (!fileSystem->DirExists(assembliesPath))
        return;

    Vector<String> results;
    fileSystem->ScanDir(results, assembliesPath, "*.dll", SCAN_FILES, true);

    // if no assembiles in Assemblies path, no need to install ClockworkNET
    if (!results.Size())
        return;

    BuildLog("Building ClockworkNET");

    fileSystem->CreateDir(buildPath_ + "/ClockworkPlayer_Resources/ClockworkNET");
    fileSystem->CreateDir(buildPath_ + "/ClockworkPlayer_Resources/ClockworkNET/Clockwork");
    fileSystem->CreateDir(buildPath_ + "/ClockworkPlayer_Resources/ClockworkNET/Clockwork/Assemblies");

    fileSystem->CopyDir(tenv->GetNETCoreCLRAbsPath(), buildPath_ + "/ClockworkPlayer_Resources/ClockworkNET/CoreCLR");
    fileSystem->CopyDir(tenv->GetNETTPAPaths(), buildPath_ + "/ClockworkPlayer_Resources/ClockworkNET/Clockwork/TPA");

    // Clockwork Assemblies

    const String& assemblyLoadPaths = tenv->GetNETAssemblyLoadPaths();
    Vector<String> paths = assemblyLoadPaths.Split(';');

    for (unsigned i = 0; i < paths.Size(); i++)
    {
        Vector<String> loadResults;
        fileSystem->ScanDir(loadResults, paths[i], "*.dll", SCAN_FILES, true);

        for (unsigned j = 0; j < loadResults.Size(); j++)
        {
            String pathName, fileName, ext;
            SplitPath(loadResults[j], pathName, fileName, ext);

            if (fileName != "ClockworkNETEngine")
                continue;

            fileSystem->Copy(paths[i] + "/" + loadResults[j], ToString("%s/ClockworkPlayer_Resources/ClockworkNET/Clockwork/Assemblies/%s.dll", buildPath_.CString(), fileName.CString()));
        }

    }

    // Project assemblied
    for (unsigned i = 0; i < results.Size(); i++)
    {
        String pathName, fileName, ext;
        SplitPath(results[i], pathName, fileName, ext);
        fileSystem->Copy(assembliesPath + results[i], ToString("%s/ClockworkPlayer_Resources/ClockworkNET/Clockwork/Assemblies/%s.dll", buildPath_.CString(), fileName.CString()));
    }



}

void BuildWindows::Build(const String& buildPath)
{
    ToolEnvironment* tenv = GetSubsystem<ToolEnvironment>();

    buildPath_ = AddTrailingSlash(buildPath) + GetBuildSubfolder();

    BuildLog("Starting Windows Deployment");

    Initialize();

    if (!BuildClean(buildPath_))
        return;

    BuildSystem* buildSystem = GetSubsystem<BuildSystem>();

    FileSystem* fileSystem = GetSubsystem<FileSystem>();

    String rootSourceDir = tenv->GetRootSourceDir();
    String playerBinary = tenv->GetPlayerBinary();
    String d3d9dll = GetPath(playerBinary) + "/D3DCompiler_47.dll";

    if (!BuildCreateDirectory(buildPath_))
        return;

    if (!BuildCreateDirectory(buildPath_ + "/ClockworkPlayer_Resources"))
        return;

    String resourcePackagePath = buildPath_ + "/ClockworkPlayer_Resources/ClockworkResources" + PAK_EXTENSION;
    GenerateResourcePackage(resourcePackagePath);
    if (buildFailed_)
        return;

    if (!BuildCreateDirectory(buildPath_ + "/Settings"))
        return;

    String engineJSON(GetSettingsDirectory() + "/Engine.json");

    if (fileSystem->FileExists(engineJSON))
    {
        if (!BuildCopyFile(engineJSON, buildPath_ + "/Settings/Engine.json"))
            return;
    }

    if (!BuildCopyFile(playerBinary, buildPath_ + "/ClockworkPlayer.exe"))
        return;

    if (!BuildCopyFile(d3d9dll, buildPath_ + "/D3DCompiler_47.dll"))
        return;

    BuildClockworkNET();

    BuildLog("Windows Deployment Complete");

    buildSystem->BuildComplete(PLATFORMID_WINDOWS, buildPath_);

}

}
