//
// Copyright (c) 2008-2014 the Urho3D project.
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
#include <Clockwork/IO/FileSystem.h>
#include <Clockwork/IO/Log.h>
#include <Clockwork/IO/IOEvents.h>
#include <Clockwork/Input/InputEvents.h>
#include <Clockwork/Input/Input.h>
#include <Clockwork/Core/Main.h>
#include <Clockwork/Core/ProcessUtils.h>
#include <Clockwork/Resource/ResourceCache.h>
#include <Clockwork/Resource/ResourceEvents.h>
#include <Clockwork/UI/UI.h>

// Move me
#include <Clockwork/Environment/Environment.h>

#include <ClockworkJS/Javascript/Javascript.h>

#include <ClockworkPlayer/Player.h>

#include <ClockworkNET/NETCore/NETCore.h>
#include "ClockworkSharpApp.h"

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

        engineParameters_["WindowTitle"] = "ClockworkPlayer";

#if (CLOCKWORK_PLATFORM_ANDROID)
        engineParameters_["FullScreen"] = true;
        engineParameters_["ResourcePaths"] = "CoreData;PlayerData;Cache;ClockworkResources";
#elif CLOCKWORK_PLATFORM_WEB
        engineParameters_["FullScreen"] = false;
        engineParameters_["ResourcePaths"] = "ClockworkResources";
        // engineParameters_["WindowWidth"] = 1280;
        // engineParameters_["WindowHeight"] = 720;
#elif CLOCKWORK_PLATFORM_IOS
        engineParameters_["FullScreen"] = false;
        engineParameters_["ResourcePaths"] = "ClockworkResources";
#else
        engineParameters_["FullScreen"] = false;
        engineParameters_["WindowWidth"] = 1280 * .55f;
        engineParameters_["WindowHeight"] = 720 * .55f;
        engineParameters_["ResourcePaths"] = "/Users/josh/Dev/clockwork/ClockworkGameEngineSharp/Resources/CoreData;/Users/josh/Dev/clockwork/ClockworkGameEngineSharp/Resources/PlayerData;/Users/josh/Dev/clockwork/ClockworkExamples/BunnyMark/Resources;/Users/josh/Dev/clockwork/ClockworkExamples/BunnyMark/";
#endif

#if CLOCKWORK_PLATFORM_WINDOWS
        engineParameters_["WindowIcon"] = "Images/ClockworkLogo32.png";
        engineParameters_["ResourcePrefixPath"] = "ClockworkPlayer_Resources";
#elif CLOCKWORK_PLATFORM_ANDROID
        //engineParameters_["ResourcePrefixPath"] = "assets";
#elif CLOCKWORK_PLATFORM_OSX
        engineParameters_["ResourcePrefixPath"] = "../Resources";
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
        engineParameters_["LogName"] = filesystem->GetAppPreferencesDir("ClockworkPlayer", "Logs") + "ClockworkPlayer.log";
    }

    void ClockworkPlayerApp::Start()
    {
        Application::Start();

        // Instantiate and register NETCore subsystem
        context_->RegisterSubsystem(new NETCore(context_));

        // Instantiate and register the Javascript subsystem
        Javascript* javascript = new Javascript(context_);
        context_->RegisterSubsystem(javascript);

        vm_ = javascript->InstantiateVM("MainVM");
        vm_->InitJSContext();

        UI* ui = GetSubsystem<UI>();
        ui->Initialize("DefaultUI/language/lng_en.tb.txt");
        ui->LoadDefaultPlayerSkin();

        vm_->SetModuleSearchPaths("Modules");

        // Instantiate and register the Player subsystem
        context_->RegisterSubsystem(new ClockworkPlayer::Player(context_));
        ClockworkPlayer::jsapi_init_clockworkplayer(vm_);

        JSVM* vm = JSVM::GetJSVM(0);

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

        context_->RemoveSubsystem<NETCore>();

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

    int ClockworkPlayerApp::Initialize()
    {
        try
        {
            Setup();
            if (exitCode_)
                return exitCode_;

            if (!engine_->Initialize(engineParameters_))
            {
                ErrorExit();
                return exitCode_;
            }

            Start();

            return exitCode_;

        }
        catch (std::bad_alloc&)
        {
            ErrorDialog(GetTypeName(), "An out-of-memory error occurred. The application will now exit.");
            return EXIT_FAILURE;
        }
    }

    bool ClockworkPlayerApp::RunFrame()
    {
        try
        {
            if (!engine_->IsExiting())
            {
                engine_->RunFrame();
                return true;
            }
            else
                return false;
        }
        catch (std::bad_alloc&)
        {
            ErrorDialog(GetTypeName(), "An out-of-memory error occurred. The application will now exit.");
            return EXIT_FAILURE;
        }
    }

    int ClockworkPlayerApp::Shutdown()
    {
        Stop();

        return exitCode_;
    }

}

static SharedPtr<Clockwork::Context> sContext;
static SharedPtr<ClockworkPlayer::ClockworkPlayerApp> sApplication;

extern "C" {

#ifdef CLOCKWORK_PLATFORM_WINDOWS
#pragma warning(disable: 4244) // possible loss of data
#define CLOCKWORK_EXPORT_API __declspec(dllexport)
#else
#define CLOCKWORK_EXPORT_API
#endif

CLOCKWORK_EXPORT_API int clockworksharp_initialize()
{
    sContext = new Clockwork::Context();
    sApplication = new ClockworkPlayer::ClockworkPlayerApp(sContext);
    return sApplication->Initialize();
}

CLOCKWORK_EXPORT_API bool clockworksharp_runframe()
{
    if (!sApplication->RunFrame())
    {
        sApplication->Shutdown();

        sApplication = 0;
        sContext = 0;
        return false;
    }

    return true;
}

}
