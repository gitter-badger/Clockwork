//
// Copyright (c) 2014-2016 CLOCKWORK STUDIOS
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

#include <Poco/File.h>

#include <Clockwork/Core/Context.h>
#include <Clockwork/Core/StringUtils.h>
#include <Clockwork/IO/FileSystem.h>
#include <Clockwork/Graphics/Graphics.h>
#include "FileUtils.h"
#include "nfd.h"

namespace ClockworkEditor
{

FileUtils::FileUtils(Context* context) :
    Object(context)
{

}

FileUtils::~FileUtils()
{
}

String FileUtils::OpenProjectFileDialog()
{
    nfdchar_t *outPath = NULL;

    nfdresult_t result = NFD_OpenDialog( "clockwork",
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

bool FileUtils::CreateDirs(const String& folder)
{
    FileSystem* fileSystem = GetSubsystem<FileSystem>();

    Poco::File dirs(folder.CString());
    dirs.createDirectories();

    return fileSystem->DirExists(folder);

}

String FileUtils::NewProjectFileDialog()
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

String FileUtils::GetBuildPath(const String& defaultPath)
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

String FileUtils::GetAndroidSDKPath(const String& defaultPath)
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

String FileUtils::GetAntPath(const String& defaultPath)
{
    String antPath;

    nfdchar_t *outPath = NULL;

#ifdef CLOCKWORK_PLATFORM_WINDOWS
    String msg = "Please select the folder which contains ant.bat";
#else
    String msg = "Please select the folder which contains the ant executable";
#endif

    nfdresult_t result = NFD_ChooseDirectory(msg.CString(),
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

String FileUtils::GetJDKRootPath(const String& defaultPath)
{
    String jdkPath;

    nfdchar_t *outPath = NULL;

    nfdresult_t result = NFD_ChooseDirectory("Please choose the root folder of your JDK",
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

String FileUtils::GetMobileProvisionPath()
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



void FileUtils::RevealInFinder(const String& fullpath)
{
    FileSystem* fs = GetSubsystem<FileSystem>();
    if (fs->DirExists(fullpath))
        fs->SystemOpen(fullpath);
    else if (fs->FileExists(fullpath))
        fs->SystemOpen(GetPath(fullpath));
}


}
