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

#include <Clockwork/Clockwork.h>
#include <Clockwork/Engine/Engine.h>
#include <Clockwork/Engine/EngineConfig.h>
#include <Clockwork/IO/FileSystem.h>
#include <Clockwork/IO/Log.h>
#include <Clockwork/IO/IOEvents.h>
#include <Clockwork/Input/InputEvents.h>
#include <Clockwork/Core/Main.h>
#include <Clockwork/Core/ProcessUtils.h>
#include <Clockwork/Resource/ResourceCache.h>
#include <Clockwork/Resource/ResourceEvents.h>
#include <Clockwork/UI/UI.h>

// Move me
#include <Clockwork/Environment/Environment.h>

#include <ClockworkJS/Javascript/Javascript.h>

#ifdef CLOCKWORK_DOTNET
#include <ClockworkNET/NETCore/NETCore.h>
#include <ClockworkNET/NETScript/NETScript.h>
#endif


#include "../PlayerMode/CEPlayerMode.h"
#include <ClockworkPlayer/Player.h>

#include "../PlayerMode/CEPlayerEvents.h"

#include "CEPlayerApp.h"

#include <Clockwork/DebugNew.h>

#ifdef __APPLE__
#include <unistd.h>
#endif

namespace ClockworkPlayer
{
    extern void jsapi_init_clockworkplayer(JSVM* vm);
}

namespace ClockworkEditor
{

CEPlayerApplication::CEPlayerApplication(Context* context) :
    CEEditorCommon(context),
    debugPlayer_(false),
    launchedByEditor_(false)
{
}

void CEPlayerApplication::Setup()
{
    CEEditorCommon::Setup();

    // Read the project engine configuration
    ReadEngineConfig();

    engine_->SetAutoExit(false);

    engineParameters_.InsertNew("WindowTitle", "ClockworkPlayer");

    // Set defaults not already set from config
#if (CLOCKWORK_PLATFORM_ANDROID)
    engineParameters_.InsertNew("FullScreen", true);
    engineParameters_.InsertNew("ResourcePaths", "CoreData;ClockworkResources");
#elif CLOCKWORK_PLATFORM_WEB
    engineParameters_.InsertNew("FullScreen", false);
    engineParameters_.InsertNew("ResourcePaths", "ClockworkResources");
    // engineParameters_.InsertNew("WindowWidth", 1280);
    // engineParameters_.InsertNew("WindowHeight", 720);
#elif CLOCKWORK_PLATFORM_IOS
    engineParameters_.InsertNew("FullScreen", false);
    engineParameters_.InsertNew("ResourcePaths", "ClockworkResources)";
#else
    engineParameters_.InsertNew("FullScreen", false);
    engineParameters_.InsertNew("WindowWidth", 1280);
    engineParameters_.InsertNew("WindowHeight", 720);
#endif

    engineParameters_.InsertNew("LogLevel", LOG_DEBUG);

#if CLOCKWORK_PLATFORM_WINDOWS || CLOCKWORK_PLATFORM_LINUX
    engineParameters_.InsertNew("WindowIcon", "Images/ClockworkLogo32.png");
    engineParameters_.InsertNew("ResourcePrefixPath", "ClockworkPlayer_Resources");
#elif CLOCKWORK_PLATFORM_ANDROID
    //engineParameters_.InsertNew("ResourcePrefixPath", "assets");
#elif CLOCKWORK_PLATFORM_OSX
    engineParameters_.InsertNew("ResourcePrefixPath", "../Resources");
#endif

    // Read command line arguments, potentially overwriting project settings
    ReadCommandLineArguments();

    // Re-apply project settings if running from editor play button
    if (launchedByEditor_)
    {
        EngineConfig::ApplyConfig(engineParameters_, true);
    }

    // Use the script file name as the base name for the log file
    engineParameters_["LogName"] = GetSubsystem<FileSystem>()->GetAppPreferencesDir("ClockworkPlayer", "Logs") + "ClockworkPlayer.log";
}

void CEPlayerApplication::ReadEngineConfig()
{
    // find the project path from the command line args

    String projectPath;
    const Vector<String>& arguments = GetArguments();

    for (unsigned i = 0; i < arguments.Size(); ++i)
    {
        if (arguments[i].Length() > 1)
        {
            String argument = arguments[i].ToLower();
            String value = i + 1 < arguments.Size() ? arguments[i + 1] : String::EMPTY;

            if (argument == "--project" && value.Length())
            {
                projectPath = AddTrailingSlash(value);
                break;
            }

        }
    }

    if (!projectPath.Length())
        return;

    String filename = projectPath + "Settings/Engine.json";

    FileSystem* fileSystem = GetSubsystem<FileSystem>();
    if (!fileSystem->FileExists(filename))
        return;

    if (EngineConfig::LoadFromFile(context_, filename))
    {
        EngineConfig::ApplyConfig(engineParameters_);
    }

}

void CEPlayerApplication::ReadCommandLineArguments()
{
    const Vector<String>& arguments = GetArguments();
    FileSystem* fileSystem = GetSubsystem<FileSystem>();

    for (unsigned i = 0; i < arguments.Size(); ++i)
    {
        if (arguments[i].Length() > 1)
        {
            String argument = arguments[i].ToLower();
            String value = i + 1 < arguments.Size() ? arguments[i + 1] : String::EMPTY;

            if (argument == "--log-std")
            {
                SubscribeToEvent(E_LOGMESSAGE, HANDLER(CEPlayerApplication, HandleLogMessage));
            }
            else if (argument.StartsWith("--ipc-server=") || argument.StartsWith("--ipc-client="))
            {
                // If we have IPC server/client we're being launched from the Editor
                // TODO: Unify this with CEPlayerMode handling
                launchedByEditor_ = true;
            }
            else if (argument == "--debug")
            {
                debugPlayer_ = true;
            }
            else if (argument == "--project" && value.Length())
            {
                engineParameters_["ResourcePrefixPath"] = "";

                value = AddTrailingSlash(value);

                // check that cache exists
                if (!fileSystem->DirExists(value + "Cache"))
                {
                    ErrorExit("Project cache folder does not exist, projects must be loaded into the Clockwork Editor at least once before using the --player command line mode");
                    return;
                }

#ifdef CLOCKWORK_DEV_BUILD

                String resourcePaths = ToString("%s/Resources/CoreData;%s/Resources/PlayerData;%sResources;%s;%sCache",
                    CLOCKWORK_ROOT_SOURCE_DIR, CLOCKWORK_ROOT_SOURCE_DIR, value.CString(), value.CString(), value.CString());

#else

#ifdef __APPLE__
                engineParameters_["ResourcePrefixPath"] = "../Resources";
#else
                engineParameters_["ResourcePrefixPath"] = fileSystem->GetProgramDir() + "Resources";
#endif

                String resourcePaths = ToString("CoreData;PlayerData;%s/;%s/Resources;%s;%sCache",
                    value.CString(), value.CString(), value.CString(), value.CString());
#endif

                LOGINFOF("Adding ResourcePaths: %s", resourcePaths.CString());

                engineParameters_["ResourcePaths"] = resourcePaths;

#ifdef CLOCKWORK_DOTNET
                NETCore* netCore = GetSubsystem<NETCore>();
                String assemblyLoadPath = GetNativePath(ToString("%sResources/Assemblies/", value.CString()));
                netCore->AddAssemblyLoadPath(assemblyLoadPath);
#endif

            }
            else if (argument == "--windowposx" && value.Length())
            {
                engineParameters_["WindowPositionX"] = atoi(value.CString());
            }
            else if (argument == "--windowposy" && value.Length())
            {
                engineParameters_["WindowPositionY"] = atoi(value.CString());
            }
            else if (argument == "--windowwidth" && value.Length())
            {
                engineParameters_["WindowWidth"] = atoi(value.CString());
            }
            else if (argument == "--windowheight" && value.Length())
            {
                engineParameters_["WindowHeight"] = atoi(value.CString());
            }
            else if (argument == "--resizable")
            {
                engineParameters_["WindowResizable"] = true;
            }
            else if (argument == "--maximize")
            {
                engineParameters_["WindowMaximized"] = true;
            }
        }
    }
}

void CEPlayerApplication::Start()
{
    CEEditorCommon::Start();

    UI* ui = GetSubsystem<UI>();
    ui->Initialize("DefaultUI/language/lng_en.tb.txt");
    ui->LoadDefaultPlayerSkin();

    context_->RegisterSubsystem(new PlayerMode(context_));
    PlayerMode* playerMode = GetSubsystem<PlayerMode>();
    playerMode->ProcessArguments();

    SubscribeToEvent(E_JSERROR, HANDLER(CEPlayerApplication, HandleJSError));

#ifdef CLOCKWORK_DOTNET
        if (debugPlayer_)
        {
           GetSubsystem<NETCore>()->WaitForDebuggerConnect();
        }
#endif

    vm_->SetModuleSearchPaths("Modules");

    // Instantiate and register the Player subsystem
    context_->RegisterSubsystem(new ClockworkPlayer::Player(context_));
    ClockworkPlayer::jsapi_init_clockworkplayer(vm_);

#ifdef CLOCKWORK_DOTNET
    // Initialize Scripting Subsystem
    NETScript* netScript = new NETScript(context_);
    context_->RegisterSubsystem(netScript);
    netScript->Initialize();
    netScript->ExecMainAssembly();
#endif

    if (!playerMode->launchedByEditor())
    {
        JSVM* vm = JSVM::GetJSVM(0);

        if (!vm->ExecuteMain())
        {
            SendEvent(E_EXITREQUESTED);
        }
    }

    SubscribeToEvent(E_PLAYERQUIT, HANDLER(CEPlayerApplication, HandleQuit));

    return;
}

void CEPlayerApplication::HandleQuit(StringHash eventType, VariantMap& eventData)
{
    engine_->Exit();
}

void CEPlayerApplication::Stop()
{
    CEEditorCommon::Stop();
}

void CEPlayerApplication::HandleScriptReloadStarted(StringHash eventType, VariantMap& eventData)
{
}

void CEPlayerApplication::HandleScriptReloadFinished(StringHash eventType, VariantMap& eventData)
{
}

void CEPlayerApplication::HandleScriptReloadFailed(StringHash eventType, VariantMap& eventData)
{
    ErrorExit();
}

void CEPlayerApplication::HandleLogMessage(StringHash eventType, VariantMap& eventData)
{
    using namespace LogMessage;

    int level = eventData[P_LEVEL].GetInt();
    // The message may be multi-line, so split to rows in that case
    Vector<String> rows = eventData[P_MESSAGE].GetString().Split('\n');

    for (unsigned i = 0; i < rows.Size(); ++i)
    {
        if (level == LOG_ERROR)
        {
            fprintf(stderr, "%s\n", rows[i].CString());
        }
        else
        {
            fprintf(stdout, "%s\n", rows[i].CString());
        }
    }

}

void CEPlayerApplication::HandleJSError(StringHash eventType, VariantMap& eventData)
{
    using namespace JSError;
    //String errName = eventData[P_ERRORNAME].GetString();
    //String errStack = eventData[P_ERRORSTACK].GetString();
    String errMessage = eventData[P_ERRORMESSAGE].GetString();
    String errFilename = eventData[P_ERRORFILENAME].GetString();
    int errLineNumber = eventData[P_ERRORLINENUMBER].GetInt();

    String errorString = ToString("%s - %s - Line: %i",
                                  errFilename.CString(), errMessage.CString(), errLineNumber);

}


}
