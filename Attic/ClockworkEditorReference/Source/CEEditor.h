// Copyright (c) 2014-2015, THUNDERBEAST GAMES LLC All rights reserved
// Please see LICENSE.md in repository root for license information
// https://github.com/ClockworkGameEngine/ClockworkGameEngine

#pragma once

#include <Clockwork/Core/Object.h>
#include "CETypes.h"

using namespace Clockwork;

namespace Clockwork
{
class Javascript;
}

namespace ClockworkEditor
{

class Project;
class MainFrame;
class CEJavascript;
class CEPlayer;
class CEPreferences;
class ResourceOps;

#ifdef USE_SPIDERMONKEY
class JSSpiderMonkeyVM;
#endif

class Editor : public Object
{
    OBJECT(Editor);

#ifdef USE_SPIDERMONKEY
    SharedPtr<JSSpiderMonkeyVM> spidermonkey_;
#endif

public:
    /// Construct.
    Editor(Context* context);
    /// Destruct.
    ~Editor();

    void LoadProject(const String& fullpath);
    void SaveProject();
    void CloseProject();
    bool IsProjectLoaded() { return project_.NotNull(); }

    void EditResource(const String& fullpath);

    MainFrame* GetMainFrame();
    Project* GetProject();
    CEJavascript* GetCEJavascript();
    CEPreferences* GetPreferences();

    void PostModalError(const String& title, const String& message);
    void PostModalInfo(const String& title, const String& message);

    bool IsPlayingProject() { return !player_.Null(); }

    void RequestPlatformChange(CEEditorPlatform platform);
    CEEditorPlatform GetCurrentPlatform() { return currentPlatform_; }

    /// Called from UI code such as MainFrame quit menu, caught in post update
    void RequestExit() { requestExit_ = true; }

private:

    void HandlePostUpdate(StringHash eventType, VariantMap& eventData);
    void HandlePlayerError(StringHash eventType, VariantMap& eventData);
    void HandlePlayRequest(StringHash eventType, VariantMap& eventData);
    void HandlePlayStarted(StringHash eventType, VariantMap& eventData);
    void HandlePlayStop(StringHash eventType, VariantMap& eventData);
    void HandlePlayStopped(StringHash eventType, VariantMap& eventData);
    void HandleExitRequested(StringHash eventType, VariantMap& eventData);

    SharedPtr<CEPlayer> player_;
    SharedPtr<Project> project_;
    SharedPtr<MainFrame> mainframe_;
    SharedPtr<Javascript> javascript_;
    SharedPtr<CEJavascript> aejavascript_;
    SharedPtr<CEPreferences> aepreferences_;

    WeakPtr<ResourceOps> resourceCreator_;

    CEEditorPlatform currentPlatform_;

    bool requestExit_;

};

/// Register Javascript library objects.
void RegisterEditorLibrary(Context* context);


}
