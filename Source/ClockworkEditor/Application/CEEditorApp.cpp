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

#include <Clockwork/Core/StringUtils.h>
#include <Clockwork/Engine/Engine.h>
#include <Clockwork/IO/FileSystem.h>
#include <Clockwork/Input/Input.h>
#include <Clockwork/Resource/ResourceCache.h>
#include <Clockwork/Graphics/Graphics.h>
#include <Clockwork/Clockwork3D/AnimatedModel.h>

#include <Clockwork/UI/UI.h>

#include <ClockworkJS/Javascript/Javascript.h>

#include <ToolCore/ToolEnvironment.h>
#include <ToolCore/License/LicenseEvents.h>
#include <ToolCore/License/LicenseSystem.h>

#include "../EditorMode/CEEditorMode.h"

#include "CEEditorApp.h"

using namespace ToolCore;

namespace ClockworkEditor
{

extern void jsapi_init_editor(JSVM* vm);

CEEditorApp::CEEditorApp(Context* context) :
    CEEditorCommon(context)
{

}

void CEEditorApp::Start()
{
    CEEditorCommon::Start();

    context_->RegisterSubsystem(new EditorMode(context_));

    vm_->SetModuleSearchPaths("ClockworkEditor/JavaScript;ClockworkEditor/EditorScripts;ClockworkEditor/EditorScripts/ClockworkEditor");

    // Do not create bone structure by default when in the editor
    // this can be toggled temporarily, for example to setup an animation preview
    AnimatedModel::SetBoneCreationEnabled(false);

    // move UI initialization to JS
    UI* ui = GetSubsystem<UI>();
    ui->Initialize("ClockworkEditor/resources/language/lng_en.tb.txt");

    SubscribeToEvent(E_JSERROR, HANDLER(CEEditorApp, HandleJSError));

    jsapi_init_editor(vm_);

    duk_get_global_string(vm_->GetJSContext(), "require");
    duk_push_string(vm_->GetJSContext(), "main");
    if (duk_pcall(vm_->GetJSContext(), 1) != 0)
    {
        vm_->SendJSErrorEvent();
        ErrorExit("Error executing main.js");
    }

    GetSubsystem<LicenseSystem>()->Initialize();

}

void CEEditorApp::Setup()
{
    context_->SetEditorContext(true);

    CEEditorCommon::Setup();

    // Ensure exclusive fullscreen is disabled in Editor application
    Input* input = GetSubsystem<Input>();
    input->SetToggleFullscreen(false);


    ToolEnvironment* env = GetSubsystem<ToolEnvironment>();

    engineParameters_["WindowTitle"] = "ClockworkEditor";
    engineParameters_["WindowResizable"] = true;
    engineParameters_["FullScreen"] = false;
    engineParameters_["LogLevel"] = LOG_DEBUG;

    FileSystem* filesystem = GetSubsystem<FileSystem>();
    engineParameters_["LogName"] = filesystem->GetAppPreferencesDir("ClockworkEditor", "Logs") + "ClockworkEditor.log";

#ifdef CLOCKWORK_PLATFORM_OSX
    engineParameters_["WindowIcon"] = "Images/ClockworkLogo32.png";
#endif

#ifdef CLOCKWORK_DEV_BUILD
    engineParameters_["ResourcePrefixPath"] = "";
    String resourcePaths = env->GetCoreDataDir() + ";" + env->GetEditorDataDir();
    // for dev builds, add the compile editor scripts from artifacts
    resourcePaths += ";" + env->GetRootSourceDir() + "Artifacts/Build/Resources/EditorData/";
    engineParameters_["ResourcePaths"] = resourcePaths;
#else

#ifdef CLOCKWORK_PLATFORM_OSX
    engineParameters_["ResourcePrefixPath"] = "../Resources";

#else
    engineParameters_["ResourcePrefixPath"] = filesystem->GetProgramDir() + "Resources";
#endif

    engineParameters_["ResourcePaths"] = "CoreData;EditorData";

#endif // CLOCKWORK_DEV_BUILD

    ReadPreferences();

}

void CEEditorApp::Stop()
{
    CEEditorCommon::Stop();
}

void CEEditorApp::HandleJSError(StringHash eventType, VariantMap& eventData)
{
    using namespace JSError;
    //String errName = eventData[P_ERRORNAME].GetString();
    String errMessage = eventData[P_ERRORMESSAGE].GetString();
    String errFilename = eventData[P_ERRORFILENAME].GetString();
    //String errStack = eventData[P_ERRORSTACK].GetString();
    int errLineNumber = vm_->GetRealLineNumber(errFilename, eventData[P_ERRORLINENUMBER].GetInt());

    String errorString = ToString("%s - %s - Line: %i", errFilename.CString(), errMessage.CString(), errLineNumber);


    ErrorExit(errorString);

}




}
