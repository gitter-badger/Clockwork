// Copyright (c) 2014-2015, THUNDERBEAST GAMES LLC All rights reserved
// Please see LICENSE.md in repository root for license information
// https://github.com/ClockworkGameEngine/ClockworkGameEngine

#pragma once

#include <Clockwork/Core/Object.h>
#include <Clockwork/IPC/IPC.h>
#include <Clockwork/IPC/IPCEvents.h>
#include <Clockwork/IPC/IPCBroker.h>

#include "../CETypes.h"

using namespace Clockwork;

namespace Clockwork
{

class JSVM;

}

namespace ClockworkEditor
{

class UIPlayer;

struct CEPlayerError
{
    String name_;
    String message_;
    String filename_;
    int lineNumber_;
    String stack_;
};

class CEPlayer : public Object
{
    OBJECT(CEPlayer);

public:
    /// Construct.
    CEPlayer(Context* context);
    /// Destruct.
    ~CEPlayer();

    bool Play(CEPlayerMode mode, const IntRect& rect = IntRect());

    CEPlayerMode GetMode() const { return mode_; }

    void Invalidate();

    bool HasErrors() { return errors_.Size() != 0; }

    const Vector<CEPlayerError>& GetErrors() { return errors_; }

    UIPlayer* GetUIPlayer() { return uiPlayer_; }
    void SetUIPlayer(UIPlayer* uiPlayer);

private:

    CEPlayerMode mode_;

    Vector<CEPlayerError> errors_;

    void HandleJSError(StringHash eventType, VariantMap& eventData);
    void HandleEditorShutdown(StringHash eventType, VariantMap& eventData);

    void HandleIPCWorkerStarted(StringHash eventType, VariantMap& eventData);
    void HandleIPCWorkerExit(StringHash eventType, VariantMap& eventData);

    SharedPtr<IPCBroker> broker_;


    SharedPtr<UIPlayer> uiPlayer_;    

};


}
