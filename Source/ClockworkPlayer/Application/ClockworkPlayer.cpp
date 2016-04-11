//
// Copyright (c) 2008-2014 the Urho3D project.
// Copyright (c) 2014-2015, CLOCKWORK STUDIOS All rights reserved
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
#include <Clockwork/Script/ScriptSystem.h>
#include <Clockwork/UI/UI.h>

// Move me
#include <Clockwork/Environment/Environment.h>

#include <ClockworkJS/Javascript/Javascript.h>

#ifdef CLOCKWORK_DOTNET
#include <ClockworkNET/NETCore/NETCore.h>
#include <ClockworkNET/NETCore/NETHost.h>
#endif

#include <ClockworkPlayer/Player.h>

#include "ClockworkPlayer.h"

#include <Clockwork/DebugNew.h>

#ifdef __APPLE__
#include <unistd.h>
#endif

DEFINE_APPLICATION_MAIN(ClockworkPlayer::ClockworkPlayerApp)

namespace ClockworkPlayer
{

extern void jsapi_init_clockworkplayer(JSVM* vm);

ClockworkPlayerApp::ClockworkPlayerApp(Context* context) :
    Application(context)
{
}

void ClockworkPlayerApp::Setup()
{

#ifdef CLOCKWORK_3D
    RegisterEnvironmentLibrary(context_);
#endif

    FileSystem* filesystem = GetSubsystem<FileSystem>();
    context_->RegisterSubsystem(new ScriptSystem(context_));

    // Read the engine configuration
    ReadEngineConfig();

    engineParameters_.InsertNew("WindowTitle", "ClockworkPlayer");

#if (CLOCKWORK_PLATFORM_ANDROID)
    engineParameters_.InsertNew("FullScreen", true);
    engineParameters_.InsertNew("ResourcePaths", "CoreData;PlayerData;Cache;ClockworkResources");
#elif CLOCKWORK_PLATFORM_WEB
    engineParameters_.InsertNew("FullScreen", false);
    engineParameters_.InsertNew("ResourcePaths", "ClockworkResources");
    // engineParameters_.InsertNew("WindowWidth", 1280);
    // engineParameters_.InsertNew("WindowHeight", 720);
#elif CLOCKWORK_PLATFORM_IOS
    engineParameters_.InsertNew("FullScreen", false);
    engineParameters_.InsertNew("ResourcePaths", "ClockworkResources");
#else
    engineParameters_.InsertNew("FullScreen", false);
    engineParameters_.InsertNew("WindowWidth", 1280);
    engineParameters_.InsertNew("WindowHeight", 720);
    engineParameters_.InsertNew("ResourcePaths", "ClockworkResources");
#endif

#if CLOCKWORK_PLATFORM_WINDOWS || CLOCKWORK_PLATFORM_LINUX

    engineParameters_.InsertNew("WindowIcon", "Images/ClockworkLogo32.png");
    engineParameters_.InsertNew("ResourcePrefixPath", "ClockworkPlayer_Resources");

#elif CLOCKWORK_PLATFORM_ANDROID
    //engineParameters_.InsertNew("ResourcePrefixPath"], "assets");
#elif CLOCKWORK_PLATFORM_OSX
    engineParameters_.InsertNew("ResourcePrefixPath", "../Resources");
#endif

    const Vector<String>& arguments = GetArguments();

    for (unsigned i = 0; i < arguments.Size(); ++i)
    {
        if (arguments[i].Length() > 1)
        {
            String argument = arguments[i].ToLower();
            String value = i + 1 < arguments.Size() ? arguments[i + 1] : String::EMPTY;

            if (argument == "--log-std")
            {
                SubscribeToEvent(E_LOGMESSAGE, HANDLER(ClockworkPlayerApp, HandleLogMessage));
            }
        }
    }

    // Use the script file name as the base name for the log file
    engineParameters_.InsertNew("LogName", filesystem->GetAppPreferencesDir("ClockworkPlayer", "Logs") + "ClockworkPlayer.log");
}

void ClockworkPlayerApp::Start()
{
    Application::Start();

    FileSystem* fileSystem = GetSubsystem<FileSystem>();

    // Instantiate and register the Javascript subsystem
    Javascript* javascript = new Javascript(context_);
    context_->RegisterSubsystem(javascript);

    vm_ = javascript->InstantiateVM("MainVM");
    vm_->InitJSContext();

    UI* ui = GetSubsystem<UI>();
    ui->Initialize("DefaultUI/language/lng_en.tb.txt");
    ui->LoadDefaultPlayerSkin();

    SubscribeToEvent(E_JSERROR, HANDLER(ClockworkPlayerApp, HandleJSError));

    vm_->SetModuleSearchPaths("Modules");

    // Instantiate and register the Player subsystem
    context_->RegisterSubsystem(new ClockworkPlayer::Player(context_));
    ClockworkPlayer::jsapi_init_clockworkplayer(vm_);

    JSVM* vm = JSVM::GetJSVM(0);

#ifdef CLOCKWORK_DOTNET

    // Instantiate and register the ClockworkNET subsystem
    SharedPtr<NETCore> netCore (new NETCore(context_));
    context_->RegisterSubsystem(netCore);
    String netCoreErrorMsg;

#ifdef CLOCKWORK_PLATFORM_WINDOWS

    String rootNETDir = fileSystem->GetProgramDir() + "ClockworkPlayer_Resources/ClockworkNET/";

#else

    String rootNETDir = fileSystem->GetProgramDir() + "ClockworkPlayer_Resources/ClockworkNET/";

#endif

    NETHost::SetCoreCLRFilesAbsPath(GetNativePath(rootNETDir + "CoreCLR/"));
    NETHost::SetCoreCLRTPAPaths(GetNativePath(rootNETDir + "Clockwork/TPA/"));
    NETHost::SetCoreCLRAssemblyLoadPaths(GetNativePath(rootNETDir + "Clockwork/Assemblies/"));

    if (!netCore->Initialize(netCoreErrorMsg))
    {
        LOGERRORF("NetCore: Unable to initialize! %s", netCoreErrorMsg.CString());
        context_->RemoveSubsystem(NETCore::GetTypeStatic());
    }
    else
    {

    }
#endif


    if (!vm->ExecuteMain())
    {
        SendEvent(E_EXITREQUESTED);
    }

    return;
}

void ClockworkPlayerApp::Stop()
{

    vm_ = 0;
    context_->RemoveSubsystem<Javascript>();
    // make sure JSVM is really down and no outstanding refs
    // as if not, will hold on engine subsystems, which is bad
    assert(!JSVM::GetJSVM(0));

    Application::Stop();


}

void ClockworkPlayerApp::HandleScriptReloadStarted(StringHash eventType, VariantMap& eventData)
{
}

void ClockworkPlayerApp::HandleScriptReloadFinished(StringHash eventType, VariantMap& eventData)
{
}

void ClockworkPlayerApp::HandleScriptReloadFailed(StringHash eventType, VariantMap& eventData)
{
    ErrorExit();
}

void ClockworkPlayerApp::HandleLogMessage(StringHash eventType, VariantMap& eventData)
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

void ClockworkPlayerApp::HandleJSError(StringHash eventType, VariantMap& eventData)
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

void ClockworkPlayerApp::ReadEngineConfig()
{
    FileSystem* fileSystem = GetSubsystem<FileSystem>();
    String filename = fileSystem->GetProgramDir() + "Settings/Engine.json";

    if (!fileSystem->FileExists(filename))
        return;

    if (EngineConfig::LoadFromFile(context_, filename))
    {
        EngineConfig::ApplyConfig(engineParameters_);
    }
}

}
