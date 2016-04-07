#pragma once

#include <Clockwork/Core/Object.h>
#include <Clockwork/IPC/IPC.h>
#include <Clockwork/IPC/IPCEvents.h>
#include <Clockwork/IPC/IPCBroker.h>

#include "../AETypes.h"

using namespace Clockwork;

namespace Clockwork
{

class JSVM;

}

namespace ClockworkEditor
{

class UIPlayer;

struct AEPlayerError
{
    String name_;
    String message_;
    String filename_;
    int lineNumber_;
    String stack_;
};

class AEPlayer : public Object
{
    OBJECT(AEPlayer);

public:
    /// Construct.
    AEPlayer(Context* context);
    /// Destruct.
    ~AEPlayer();

    bool Play(AEPlayerMode mode, const IntRect& rect = IntRect());

    AEPlayerMode GetMode() const { return mode_; }

    void Invalidate();

    bool HasErrors() { return errors_.Size() != 0; }

    const Vector<AEPlayerError>& GetErrors() { return errors_; }

    UIPlayer* GetUIPlayer() { return uiPlayer_; }
    void SetUIPlayer(UIPlayer* uiPlayer);

private:

    AEPlayerMode mode_;

    Vector<AEPlayerError> errors_;

    void HandleJSError(StringHash eventType, VariantMap& eventData);
    void HandleEditorShutdown(StringHash eventType, VariantMap& eventData);

    void HandleIPCWorkerStarted(StringHash eventType, VariantMap& eventData);
    void HandleIPCWorkerExit(StringHash eventType, VariantMap& eventData);

    SharedPtr<IPCBroker> broker_;


    SharedPtr<UIPlayer> uiPlayer_;    

};


}
