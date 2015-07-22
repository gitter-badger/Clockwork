

#pragma once

#include <Clockwork/Engine/Application.h>

using namespace Clockwork;

/// ClockworkPlayer application runs a script specified on the command line.
class ClockworkPlayer : public Application
{
    OBJECT(ClockworkPlayer);

public:
    /// Construct.
    ClockworkPlayer(Context* context);

    /// Setup before engine initialization. Verify that a script file has been specified.
    virtual void Setup();
    /// Setup after engine initialization. Load the script and execute its start function.
    virtual void Start();
    /// Cleanup after the main loop. Run the script's stop function if it exists.
    virtual void Stop();

private:
    /// Handle reload start of the script file.
    void HandleScriptReloadStarted(StringHash eventType, VariantMap& eventData);
    /// Handle reload success of the script file.
    void HandleScriptReloadFinished(StringHash eventType, VariantMap& eventData);
    /// Handle reload failure of the script file.
    void HandleScriptReloadFailed(StringHash eventType, VariantMap& eventData);

    /// Script file name.
    String scriptFileName_;
    
#ifdef CLOCKWORK_ANGELSCRIPT
    /// Script file.
    SharedPtr<ScriptFile> scriptFile_;
#endif
};
