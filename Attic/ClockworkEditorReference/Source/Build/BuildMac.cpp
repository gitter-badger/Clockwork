// Copyright (c) 2014-2015, THUNDERBEAST GAMES LLC All rights reserved
// Please see LICENSE.md in repository root for license information
// https://github.com/ClockworkGameEngine/ClockworkGameEngine

#include "ClockworkEditor.h"
#include <Clockwork/IO/FileSystem.h>
#include "../CEEditor.h"
#include "../Project/CEProject.h"
#include "../Project/ProjectUtils.h"
#include "License/CELicenseSystem.h"

#include "BuildMac.h"
#include "BuildSystem.h"

namespace ClockworkEditor
{

BuildMac::BuildMac(Context * context) : BuildBase(context)
{

}

BuildMac::~BuildMac()
{

}

void BuildMac::Initialize()
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

void BuildMac::Build(const String& buildPath)
{
    buildPath_ = buildPath + "/Mac-Build";

    Initialize();

    BuildSystem* buildSystem = GetSubsystem<BuildSystem>();

// BEGIN LICENSE MANAGEMENT
    LicenseSystem *licenseSystem = GetSubsystem<LicenseSystem>();
    if (licenseSystem->IsStandardLicense())
    {
        if (containsMDL_)
        {
            buildSystem->BuildComplete(CE_PLATFORM_MAC, buildPath_, false, true);
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

    buildSourceDir += "Deployment/MacOS/ClockworkPlayer.app";

    fileSystem->CreateDir(buildPath_);

    buildPath_ += "/ClockworkPlayer.app";

    fileSystem->CreateDir(buildPath_);

    fileSystem->CreateDir(buildPath_ + "/Contents");
    fileSystem->CreateDir(buildPath_ + "/Contents/MacOS");
    fileSystem->CreateDir(buildPath_ + "/Contents/Resources");

    String resourcePackagePath = buildPath_ + "/Contents/Resources/ClockworkResources.pak";
    GenerateResourcePackage(resourcePackagePath);

    fileSystem->Copy(buildSourceDir + "/Contents/Resources/Clockwork.icns", buildPath_ + "/Contents/Resources/Clockwork.icns");

    fileSystem->Copy(buildSourceDir + "/Contents/Info.plist", buildPath_ + "/Contents/Info.plist");
    fileSystem->Copy(buildSourceDir + "/Contents/MacOS/ClockworkPlayer", buildPath_ + "/Contents/MacOS/ClockworkPlayer");

#ifdef CLOCKWORK_PLATFORM_OSX
    Vector<String> args;
    args.Push("+x");
    args.Push(buildPath_ + "/Contents/MacOS/ClockworkPlayer");
    fileSystem->SystemRun("chmod", args);
#endif

    buildPath_ = buildPath + "/Mac-Build";    
    buildSystem->BuildComplete(CE_PLATFORM_MAC, buildPath_);

}

}
