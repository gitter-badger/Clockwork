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

#include <Clockwork/Engine/Engine.h>
#include <Clockwork/Input/Input.h>
#include <Clockwork/Graphics/Graphics.h>

#include <Clockwork/IPC/IPC.h>

// Move me to Engine
#include <Clockwork/Environment/Environment.h>

#include <Clockwork/Script/ScriptSystem.h>
#include <ClockworkJS/Javascript/Javascript.h>

#include <ToolCore/ToolSystem.h>
#include <ToolCore/ToolEnvironment.h>

#include <Clockwork/IO/File.h>

#ifdef CLOCKWORK_DOTNET
#include <ClockworkNET/NETCore/NETHost.h>
#include <ClockworkNET/NETCore/NETCore.h>
#include <ClockworkNET/NETScript/NETScript.h>
#endif

#ifdef CLOCKWORK_WEBVIEW
#include <ClockworkWebView/WebBrowserHost.h>
#endif

#include "../Components/EditorComponents.h"

#include "CEEditorCommon.h"

namespace Clockwork
{
    void jsapi_init_clockworknet(JSVM* vm);
    void jsapi_init_webview(JSVM* vm);;
}

using namespace ToolCore;

namespace ToolCore
{
    extern void jsapi_init_toolcore(JSVM* vm);
}


namespace ClockworkEditor
{

CEEditorCommon::CEEditorCommon(Context* context) :
    Application(context)
{

}

void CEEditorCommon::Start()
{
    ValidateWindow();

    Input* input = GetSubsystem<Input>();
    input->SetMouseVisible(true);

    Javascript* javascript = GetSubsystem<Javascript>();
    vm_ = javascript->InstantiateVM("MainVM");
    vm_->InitJSContext();

    jsapi_init_toolcore(vm_);

#ifdef CLOCKWORK_WEBVIEW
    // Initialize in Start so window already exists
    context_->RegisterSubsystem(new WebBrowserHost(context_));
    jsapi_init_webview(vm_);
#endif


#ifdef CLOCKWORK_DOTNET
    jsapi_init_clockworknet(vm_);
#endif



}

void CEEditorCommon::Setup()
{

#ifdef CLOCKWORK_3D
    RegisterEnvironmentLibrary(context_);
#endif

    RegisterEditorComponentLibrary(context_);

#ifdef CLOCKWORK_DOTNET
    RegisterNETScriptLibrary(context_);
#endif

    // Register IPC system
    context_->RegisterSubsystem(new IPC(context_));

    context_->RegisterSubsystem(new ScriptSystem(context_));

    // Instantiate and register the Javascript subsystem
    Javascript* javascript = new Javascript(context_);
    context_->RegisterSubsystem(javascript);

    ToolEnvironment* env = new ToolEnvironment(context_);
    context_->RegisterSubsystem(env);

#ifdef CLOCKWORK_DEV_BUILD

    if (!env->InitFromJSON())
    {
        ErrorExit(ToString("Unable to initialize tool environment from %s", env->GetDevConfigFilename().CString()));
        return;
    }
#else

    env->InitFromPackage();

#endif

#ifdef CLOCKWORK_DOTNET

    // Instantiate and register the ClockworkNET subsystem
    SharedPtr<NETCore> netCore (new NETCore(context_));
    context_->RegisterSubsystem(netCore);
    String netCoreErrorMsg;

    NETHost::SetCoreCLRFilesAbsPath(env->GetNETCoreCLRAbsPath());
    NETHost::SetCoreCLRTPAPaths(env->GetNETTPAPaths());
    NETHost::SetCoreCLRAssemblyLoadPaths(env->GetNETAssemblyLoadPaths());

    if (!netCore->Initialize(netCoreErrorMsg))
    {
        LOGERRORF("NetCore: Unable to initialize! %s", netCoreErrorMsg.CString());
        context_->RemoveSubsystem(NETCore::GetTypeStatic());
    }
    else
    {

    }
#endif

    ToolSystem* system = new ToolSystem(context_);
    context_->RegisterSubsystem(system);

}

void CEEditorCommon::Stop()
{

    context_->RemoveSubsystem<IPC>();

    vm_ = 0;
    context_->RemoveSubsystem<Javascript>();
    // make sure JSVM is really down and no outstanding refs
    // as if not, will hold on engine subsystems, which is bad
    assert(!JSVM::GetJSVM(0));

#ifdef CLOCKWORK_DOTNET
    NETCore* netCore = GetSubsystem<NETCore>();
    if (netCore)
    {
        netCore->Shutdown();
        context_->RemoveSubsystem<NETCore>();
    }
#endif
}

bool CEEditorCommon::CreateDefaultPreferences(String& path, JSONValue& prefs)
{
    // Note there is some duplication here with the editor's
    // TypeScript preference code, this is due to the preferences for
    // the editor window needing to be available at window creation time
    // It could be better to split this all out to a native, scriptable
    // preferences object

    LOGINFOF("Creating default Clockwork Editor preferences: %s", path.CString());

    SharedPtr<JSONFile> jsonFile(new JSONFile(context_));

    JSONValue& root = jsonFile->GetRoot();

    root.Clear();
    root["recentProjects"] = JSONArray();

    JSONValue editorWindow;
    GetDefaultWindowPreferences(editorWindow, true);

    JSONValue playerWindow;
    GetDefaultWindowPreferences(playerWindow, false);

    root["editorWindow"] = editorWindow;
    root["playerWindow"] = playerWindow;

    prefs = root;

    SavePreferences(prefs);

    return true;
}

bool CEEditorCommon::ReadPreferences()
{
    FileSystem* fileSystem = GetSubsystem<FileSystem>();
    String path = GetPreferencesPath();

    JSONValue prefs;

    LoadPreferences(prefs);

    if (!prefs.IsObject() || !prefs["editorWindow"].IsObject())
    {
        if (!CreateDefaultPreferences(path, prefs))
            return false;
    }

    JSONValue& editorWindow = prefs["editorWindow"];

    engineParameters_["WindowPositionX"] = editorWindow["x"].GetUInt();
    engineParameters_["WindowPositionY"] = editorWindow["y"].GetUInt();
    engineParameters_["WindowWidth"] = editorWindow["width"].GetUInt();
    engineParameters_["WindowHeight"] = editorWindow["height"].GetUInt();
    engineParameters_["WindowMaximized"] = editorWindow["maximized"].GetBool();

    return true;
}

void CEEditorCommon::ValidateWindow()
{
    Graphics* graphics = GetSubsystem<Graphics>();
    IntVector2 windowPosition = graphics->GetWindowPosition();
    int monitors = graphics->GetNumMonitors();
    IntVector2 maxResolution;

    for (int i = 0; i < monitors; i++)
    {
        IntVector2 monitorResolution = graphics->GetMonitorResolution(i);
        maxResolution += monitorResolution;
    }

    if (windowPosition.x_ >= maxResolution.x_ || windowPosition.y_ >= maxResolution.y_ || (windowPosition.x_ + graphics->GetWidth()) < 0 || (windowPosition.y_ + graphics->GetHeight()) < 0)
    {
        JSONValue prefs;

        if (!LoadPreferences(prefs))
            return;

        bool editor = context_->GetEditorContext();

        JSONValue window;
        GetDefaultWindowPreferences(window, editor);

        prefs[editor ? "editorWindow" : "playerWindow"] = window;

        //Setting the mode to 0 width/height will use engine defaults for window size and layout
        graphics->SetMode(0, 0, graphics->GetFullscreen(), graphics->GetBorderless(), graphics->GetResizable(), graphics->GetVSync(), graphics->GetTripleBuffer(), graphics->GetMultiSample(), editor);

        SavePreferences(prefs);
    }
}

void CEEditorCommon::GetDefaultWindowPreferences(JSONValue& windowPrefs, bool maximized)
{
    windowPrefs["x"] = 0;
    windowPrefs["y"] = 0;
    windowPrefs["width"] = 0;
    windowPrefs["height"] = 0;
    windowPrefs["monitor"] = 0;
    windowPrefs["maximized"] = maximized;
}

String CEEditorCommon::GetPreferencesPath()
{
    FileSystem* fileSystem = GetSubsystem<FileSystem>();
    String path = fileSystem->GetAppPreferencesDir("ClockworkEditor", "Preferences");
    path += "prefs.json";
    return path;
}

bool CEEditorCommon::LoadPreferences(JSONValue& prefs)
{
    FileSystem* fileSystem = GetSubsystem<FileSystem>();
    String path = GetPreferencesPath();

    if (!fileSystem->FileExists(path))
    {
        if (!CreateDefaultPreferences(path, prefs))
            return false;
    }
    else
    {
        SharedPtr<File> file(new File(context_, path, FILE_READ));
        SharedPtr<JSONFile> jsonFile(new JSONFile(context_));

        if (!jsonFile->BeginLoad(*file))
        {
            file->Close();
            if (!CreateDefaultPreferences(path, prefs))
                return false;
        }
        else
        {
            prefs = jsonFile->GetRoot();
        }

        file->Close();
    }

    return true;
}

bool CEEditorCommon::SavePreferences(JSONValue& prefs)
{
    FileSystem* fileSystem = GetSubsystem<FileSystem>();
    String path = GetPreferencesPath();

    SharedPtr<File> file(new File(context_, path, FILE_WRITE));
    SharedPtr<JSONFile> jsonFile(new JSONFile(context_));

    jsonFile->GetRoot() = prefs;

    if (!file->IsOpen())
    {
        LOGERRORF("Unable to open Clockwork Editor preferences for writing: %s", path.CString());
        return false;
    }

    jsonFile->Save(*file, "   ");
    file->Close();

    return true;
}

}
