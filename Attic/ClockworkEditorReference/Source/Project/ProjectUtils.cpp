// Copyright (c) 2014-2015, THUNDERBEAST GAMES LLC All rights reserved
// Please see LICENSE.md in repository root for license information
// https://github.com/ClockworkGameEngine/ClockworkGameEngine

#include "ClockworkEditor.h"

#include <Clockwork/Core/Context.h>
#include <Clockwork/IO/FileSystem.h>
#include <Clockwork/Graphics/Graphics.h>
#include "ProjectUtils.h"
#include "CEEvents.h"
#include "nfd.h"

#include "../CEEditor.h"

namespace ClockworkEditor
{

ProjectUtils::ProjectUtils(Context* context) :
    Object(context)
{
     SubscribeToEvent(E_EDITORSHUTDOWN, HANDLER(ProjectUtils, HandleEditorShutdown));
}

void ProjectUtils::OpenProjectFileDialog()
{
    nfdchar_t *outPath = NULL;

    nfdresult_t result = NFD_OpenDialog( "clockwork",
                                NULL,
                                &outPath);

    if (outPath && result == NFD_OKAY)
    {
        String fullpath = outPath;
        Editor* editor = GetSubsystem<Editor>();
        editor->LoadProject(fullpath);
    }

    GetSubsystem<Graphics>()->RaiseWindow();

    if (outPath)
        free(outPath);

}

String ProjectUtils::GetBuildPath(const String& defaultPath)
{
    String buildPath;

    nfdchar_t *outPath = NULL;

    nfdresult_t result = NFD_ChooseDirectory( "Please choose the build folder",
                                defaultPath.Length() ? defaultPath.CString() : NULL,
                                &outPath);

    if (outPath && result == NFD_OKAY)
    {
        buildPath = outPath;
    }

    if (outPath)
        free(outPath);

    GetSubsystem<Graphics>()->RaiseWindow();

    return GetInternalPath(buildPath);

}

String ProjectUtils::GetMobileProvisionPath()
{
    nfdchar_t *outPath = NULL;

    nfdresult_t result = NFD_OpenDialog( "mobileprovision",
                                NULL,
                                &outPath);

    String fullpath;

    if (outPath && result == NFD_OKAY)
    {
         fullpath = outPath;
    }

    GetSubsystem<Graphics>()->RaiseWindow();

    if (outPath)
        free(outPath);

    return fullpath;

}

String ProjectUtils::GetAndroidSDKPath(const String& defaultPath)
{
    String sdkPath;

    nfdchar_t *outPath = NULL;

    nfdresult_t result = NFD_ChooseDirectory( "Please choose the root folder of your Android SDK",
                                defaultPath.Length() ? defaultPath.CString() : NULL,
                                &outPath);

    if (outPath && result == NFD_OKAY)
    {
        sdkPath = outPath;
    }

    if (outPath)
        free(outPath);

    GetSubsystem<Graphics>()->RaiseWindow();

    return GetInternalPath(sdkPath);

}

String ProjectUtils::GetAntPath(const String& defaultPath)
{
    String antPath;

    nfdchar_t *outPath = NULL;

#ifdef CLOCKWORK_PLATFORM_WINDOWS
    String msg = "Please select the folder which contains ant.bat";
#else
     String msg = "Please select the folder which contains the ant executable";
#endif

    nfdresult_t result = NFD_ChooseDirectory( msg.CString(),
                                defaultPath.Length() ? defaultPath.CString() : NULL,
                                &outPath);

    if (outPath && result == NFD_OKAY)
    {
        antPath = outPath;
    }

    if (outPath)
        free(outPath);

    GetSubsystem<Graphics>()->RaiseWindow();

    return GetInternalPath(antPath);
}

String ProjectUtils::GetJDKRootPath(const String& defaultPath)
{
    String jdkPath;

    nfdchar_t *outPath = NULL;

    nfdresult_t result = NFD_ChooseDirectory( "Please choose the root folder of your JDK",
                                defaultPath.Length() ? defaultPath.CString() : NULL,
                                &outPath);

    if (outPath && result == NFD_OKAY)
    {
        jdkPath = outPath;
    }

    if (outPath)
        free(outPath);

    GetSubsystem<Graphics>()->RaiseWindow();

    return GetInternalPath(jdkPath);

}



String ProjectUtils::NewProjectFileDialog()
{

    String projectPath;

    nfdchar_t *outPath = NULL;

    nfdresult_t result = NFD_ChooseDirectory( "Please choose the root folder for your project",
                                NULL,
                                &outPath);


    if (outPath && result == NFD_OKAY)
    {
        projectPath = outPath;
    }

    GetSubsystem<Graphics>()->RaiseWindow();

    if (outPath)
        free(outPath);

    return projectPath;

}

void ProjectUtils::RevealInFinder(const String& fullpath)
{
    FileSystem* fs = GetSubsystem<FileSystem>();
    if (fs->DirExists(fullpath))
        fs->SystemOpen(fullpath);
    else if (fs->FileExists(fullpath))
        fs->SystemOpen(GetPath(fullpath));
}

ProjectUtils::~ProjectUtils()
{
}

void ProjectUtils::HandleEditorShutdown(StringHash eventType, VariantMap& eventData)
{
    context_->RemoveSubsystem(GetType());
}


}
