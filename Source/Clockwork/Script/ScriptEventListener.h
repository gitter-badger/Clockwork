#pragma once

#include "../Core/Object.h"

class asILockableSharedBool;
class asIScriptFunction;
class asIScriptObject;

namespace Clockwork
{

/// Delay-executed function or method call.
struct DelayedCall
{
    /// Period for repeating calls.
    float period_;
    /// Delay time remaining until execution.
    float delay_;
    /// Repeat flag.
    bool repeat_;
    /// Function declaration.
    String declaration_;
    /// Parameters.
    VariantVector parameters_;
};

/// Interface class for allowing script objects or functions to subscribe to events.
class CLOCKWORK_API ScriptEventListener
{
public:
    /// Destruct
    virtual ~ScriptEventListener() {};

    /// Add a scripted event handler.
    virtual void AddEventHandler(StringHash eventType, const String& handlerName) = 0;
    /// Add a scripted event handler for a specific sender.
    virtual void AddEventHandler(Object* sender, StringHash eventType, const String& handlerName) = 0;
    /// Remove a scripted event handler.
    virtual void RemoveEventHandler(StringHash eventType) = 0;
    /// Remove a scripted event handler for a specific sender.
    virtual void RemoveEventHandler(Object* sender, StringHash eventType) = 0;
    /// Remove all scripted event handlers for a specific sender.
    virtual void RemoveEventHandlers(Object* sender) = 0;
    /// Remove all scripted event handlers.
    virtual void RemoveEventHandlers() = 0;
    /// Remove all scripted event handlers, except those listed.
    virtual void RemoveEventHandlersExcept(const PODVector<StringHash>& exceptions) = 0;
};

}
