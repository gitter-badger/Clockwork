// Copyright (c) 2014-2015, THUNDERBEAST GAMES LLC All rights reserved
// Please see LICENSE.md in repository root for license information
// https://github.com/ClockworkGameEngine/ClockworkGameEngine

#include "ClockworkEditor.h"
#include <Clockwork/IO/FileSystem.h>
#include "../CEEditor.h"
#include "../Project/CEProject.h"

#include "../Project/ProjectUtils.h"
#include "License/CELicenseSystem.h"

#include "BuildSystem.h"
#include "BuildWindows.h"

namespace ClockworkEditor
{

BuildWindows::BuildWindows(Context * context) : BuildBase(context)
{

}

BuildWindows::~BuildWindows()
{

}

void BuildWindows::Initialize()
{
    Editor* editor = GetSubsystem<Editor>();
    Project* project = editor->GetProject();

    FileSystem* fileSystem = GetSubsystem<FileSystem>();

#ifdef CLOCKWORK_PLATFORM_WINDOWS
    String bundleResources = fileSystem->GetProgramDir();
#else
    String bundleResources = fileSystem->GetAppBundleResourceFolder();
#endif

    String projectResources = project->GetResourcePath();
    String coreDataFolder = bundleResources + "CoreData/";

    AddResourceDir(coreDataFolder);
    AddResourceDir(projectResources);

    BuildResourceEntries();

}

void BuildWindows::Build(const String& buildPath)
{
    buildPath_ = buildPath + "/Windows-Build";

    Initialize();

    BuildSystem* buildSystem = GetSubsystem<BuildSystem>();

// BEGIN LICENSE MANAGEMENT
    LicenseSystem *licenseSystem = GetSubsystem<LicenseSystem>();
    if (licenseSystem->IsStandardLicense())
    {
        if (containsMDL_)
        {
            buildSystem->BuildComplete(CE_PLATFORM_WINDOWS, buildPath_, false, true);
            return;
        }
    }
// END LICENSE MANAGEMENT

    FileSystem* fileSystem = GetSubsystem<FileSystem>();
    if (fileSystem->DirExists(buildPath_))
        fileSystem->RemoveDir(buildPath_, true);

#ifdef CLOCKWORK_PLATFORM_WINDOWS
    String buildSourceDir = fileSystem->GetProgramDir();
#else
    String buildSourceDir = fileSystem->GetAppBundleResourceFolder();
#endif

    buildSourceDir += "Deployment/Win64";

    fileSystem->CreateDir(buildPath_);
    fileSystem->CreateDir(buildPath_ + "/ClockworkPlayer_Resources");

    String resourcePackagePath = buildPath_ + "/ClockworkPlayer_Resources/ClockworkResources.pak";
    GenerateResourcePackage(resourcePackagePath);

    fileSystem->Copy(buildSourceDir + "/ClockworkPlayer.exe", buildPath_ + "/ClockworkPlayer.exe");
    fileSystem->Copy(buildSourceDir + "/D3DCompiler_47.dll", buildPath_ + "/D3DCompiler_47.dll");

    buildSystem->BuildComplete(CE_PLATFORM_WINDOWS, buildPath_);

}

}
