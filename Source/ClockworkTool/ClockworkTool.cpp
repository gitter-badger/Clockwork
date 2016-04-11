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

#include <Clockwork/Core/ProcessUtils.h>
#include <Clockwork/IO/Log.h>
#include <Clockwork/IO/FileSystem.h>
#include <Clockwork/Engine/Engine.h>
#include <Clockwork/Resource/ResourceCache.h>

#include <ToolCore/ToolSystem.h>
#include <ToolCore/ToolEnvironment.h>
#include <ToolCore/Build/BuildSystem.h>
#include <ToolCore/License/LicenseEvents.h>
#include <ToolCore/License/LicenseSystem.h>
#include <ToolCore/Command/Command.h>
#include <ToolCore/Command/CommandParser.h>

#include "ClockworkTool.h"

DEFINE_APPLICATION_MAIN(ClockworkTool::ClockworkTool);

using namespace ToolCore;

namespace ClockworkTool
{

ClockworkTool::ClockworkTool(Context* context) :
    Application(context),
    deactivate_(false)
{

}

ClockworkTool::~ClockworkTool()
{

}

void ClockworkTool::Setup()
{
    const Vector<String>& arguments = GetArguments();

    for (unsigned i = 0; i < arguments.Size(); ++i)
    {
        if (arguments[i].Length() > 1)
        {
            String argument = arguments[i].ToLower();
            String value = i + 1 < arguments.Size() ? arguments[i + 1] : String::EMPTY;

            if (argument == "--cli-data-path")
            {
                if (!value.Length())
                    ErrorExit("Unable to parse --cli-data-path");

                cliDataPath_ = AddTrailingSlash(value);
            }
            else if (argument == "--activate")
            {
                if (!value.Length())
                    ErrorExit("Unable to parse --activation product key");

                activationKey_ = value;
            }
            else if (argument == "--deactivate")
            {
                deactivate_ = true;
            }

        }

    }

    engineParameters_["Headless"] = true;
    engineParameters_["LogLevel"] = LOG_INFO;

    // no default resources, ClockworkTool may be run outside of source tree
    engineParameters_["ResourcePaths"] = "";
}

void ClockworkTool::HandleCommandFinished(StringHash eventType, VariantMap& eventData)
{
    GetSubsystem<Engine>()->Exit();
}

void ClockworkTool::HandleCommandError(StringHash eventType, VariantMap& eventData)
{
    String error = "Command Error";

    const String& message = eventData[CommandError::P_MESSAGE].ToString();
    if (message.Length())
        error = message;

    ErrorExit(error);
}

void ClockworkTool::HandleLicenseEulaRequired(StringHash eventType, VariantMap& eventData)
{
    ErrorExit("\nActivation Required: Please run: clockwork-cli activate\n");
}

void ClockworkTool::HandleLicenseActivationRequired(StringHash eventType, VariantMap& eventData)
{
    ErrorExit("\nActivation Required: Please run: clockwork-cli activate\n");
}

void ClockworkTool::HandleLicenseSuccess(StringHash eventType, VariantMap& eventData)
{
    if (command_.Null())
    {
        GetSubsystem<Engine>()->Exit();
        return;
    }

    command_->Run();
}

void ClockworkTool::HandleLicenseError(StringHash eventType, VariantMap& eventData)
{
    ErrorExit("\nActivation Required: Please run: clockwork-cli activate\n");
}

void ClockworkTool::HandleLicenseActivationError(StringHash eventType, VariantMap& eventData)
{
    String message = eventData[LicenseActivationError::P_MESSAGE].ToString();
    ErrorExit(message);
}

void ClockworkTool::HandleLicenseActivationSuccess(StringHash eventType, VariantMap& eventData)
{
    LOGRAW("\nActivation successful, thank you!\n\n");
    GetSubsystem<Engine>()->Exit();
}

void ClockworkTool::DoActivation()
{
    LicenseSystem* licenseSystem = GetSubsystem<LicenseSystem>();

    if (!licenseSystem->ValidateKey(activationKey_))
    {
        ErrorExit(ToString("\nProduct key \"%s\" is invalid, keys are in the form CLOCKWORK-XXXX-XXXX-XXXX-XXXX\n", activationKey_.CString()));
        return;
    }

    licenseSystem->LicenseAgreementConfirmed();

    SubscribeToEvent(E_LICENSE_ACTIVATIONERROR, HANDLER(ClockworkTool, HandleLicenseActivationError));
    SubscribeToEvent(E_LICENSE_ACTIVATIONSUCCESS, HANDLER(ClockworkTool, HandleLicenseActivationSuccess));

    licenseSystem->RequestServerActivation(activationKey_);

}

void ClockworkTool::HandleLicenseDeactivationError(StringHash eventType, VariantMap& eventData)
{
    String message = eventData[LicenseDeactivationError::P_MESSAGE].ToString();
    ErrorExit(message);
}

void ClockworkTool::HandleLicenseDeactivationSuccess(StringHash eventType, VariantMap& eventData)
{
    LOGRAW("\nDeactivation successful\n\n");
    GetSubsystem<Engine>()->Exit();
}

void ClockworkTool::DoDeactivation()
{
    LicenseSystem* licenseSystem = GetSubsystem<LicenseSystem>();

    if (!licenseSystem->LoadLicense())
    {
        ErrorExit("\nNot activated");
        return;
    }

    if (!licenseSystem->Deactivate())
    {
        ErrorExit("\nNot activated\n");
        return;
    }

    SubscribeToEvent(E_LICENSE_DEACTIVATIONERROR, HANDLER(ClockworkTool, HandleLicenseDeactivationError));
    SubscribeToEvent(E_LICENSE_DEACTIVATIONSUCCESS, HANDLER(ClockworkTool, HandleLicenseDeactivationSuccess));
}

void ClockworkTool::Start()
{
    // Subscribe to events
    SubscribeToEvent(E_COMMANDERROR, HANDLER(ClockworkTool, HandleCommandError));
    SubscribeToEvent(E_COMMANDFINISHED, HANDLER(ClockworkTool, HandleCommandFinished));

    SubscribeToEvent(E_LICENSE_EULAREQUIRED, HANDLER(ClockworkTool, HandleLicenseEulaRequired));
    SubscribeToEvent(E_LICENSE_ACTIVATIONREQUIRED, HANDLER(ClockworkTool, HandleLicenseActivationRequired));
    SubscribeToEvent(E_LICENSE_ERROR, HANDLER(ClockworkTool, HandleLicenseError));
    SubscribeToEvent(E_LICENSE_SUCCESS, HANDLER(ClockworkTool, HandleLicenseSuccess));

    const Vector<String>& arguments = GetArguments();

    ToolSystem* tsystem = new ToolSystem(context_);
    context_->RegisterSubsystem(tsystem);

    ToolEnvironment* env = new ToolEnvironment(context_);
    context_->RegisterSubsystem(env);

//#ifdef CLOCKWORK_DEV_BUILD

    if (!env->InitFromJSON())
    {
        ErrorExit(ToString("Unable to initialize tool environment from %s", env->GetDevConfigFilename().CString()));
        return;
    }

    if (!cliDataPath_.Length())
    {
        cliDataPath_ = env->GetRootSourceDir() + "/Resources/";
    }

//#endif

    tsystem->SetCLI();
    tsystem->SetDataPath(cliDataPath_);


    if (activationKey_.Length())
    {
        DoActivation();
        return;
    } else if (deactivate_)
    {
        DoDeactivation();
        return;
    }

    BuildSystem* buildSystem = GetSubsystem<BuildSystem>();

    SharedPtr<CommandParser> parser(new CommandParser(context_));

    SharedPtr<Command> cmd(parser->Parse(arguments));
    if (!cmd)
    {
        String error = "No command found";

        if (parser->GetErrorMessage().Length())
            error = parser->GetErrorMessage();

        ErrorExit(error);
        return;
    }

    if (cmd->RequiresProjectLoad())
    {
        FileSystem* fileSystem = GetSubsystem<FileSystem>();

        String projectDirectory = fileSystem->GetCurrentDir();

        Vector<String> projectFiles;
        fileSystem->ScanDir(projectFiles, projectDirectory, "*.clockwork", SCAN_FILES, false);
        if (!projectFiles.Size())
        {
            ErrorExit(ToString("No .clockwork project file in %s", projectDirectory.CString()));
            return;
        }
        else if (projectFiles.Size() > 1)
        {
            ErrorExit(ToString("Multiple .clockwork project files found in %s", projectDirectory.CString()));
            return;
        }

        String projectFile = projectDirectory + "/" + projectFiles[0];

        if (!tsystem->LoadProject(projectFile))
        {
            //ErrorExit(ToString("Failed to load project: %s", projectFile.CString()));
            //return;
        }

        // Set the build path
        String buildFolder = projectDirectory + "/" + "Build";
        buildSystem->SetBuildPath(buildFolder);

        if (!fileSystem->DirExists(buildFolder))
        {
            fileSystem->CreateDir(buildFolder);

            if (!fileSystem->DirExists(buildFolder))
            {
                ErrorExit(ToString("Failed to create build folder: %s", buildFolder.CString()));
                return;
            }
        }

    }

    command_ = cmd;

    // BEGIN LICENSE MANAGEMENT
    if (cmd->RequiresLicenseValidation())
    {
        GetSubsystem<LicenseSystem>()->Initialize();
    }
    else
    {
        if (command_.Null())
        {
            GetSubsystem<Engine>()->Exit();
            return;
        }

        command_->Run();
    }
    // END LICENSE MANAGEMENT

}

void ClockworkTool::Stop()
{

}

void ClockworkTool::ErrorExit(const String& message)
{
    engine_->Exit(); // Close the rendering window
    exitCode_ = EXIT_FAILURE;

    // Only for WIN32, otherwise the error messages would be double posted on Mac OS X and Linux platforms
    if (!message.Length())
    {
        #ifdef WIN32
        Clockwork::ErrorExit(startupErrors_.Length() ? startupErrors_ :
            "Application has been terminated due to unexpected error.", exitCode_);
        #endif
    }
    else
        Clockwork::ErrorExit(message, exitCode_);
}



}
