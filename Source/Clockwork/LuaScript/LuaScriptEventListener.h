#pragma once

#include "../Core/Object.h"

namespace Clockwork
{

/// Lua script event listener.
class CLOCKWORK_API LuaScriptEventListener
{
public:
    /// Destruct.
    virtual ~LuaScriptEventListener() {};

    /// Add a scripted event handler by function.
    virtual void AddEventHandler(const String& eventName, int functionIndex) = 0;
    /// Add a scripted event handler by function name.
    virtual void AddEventHandler(const String& eventName, const String& functionName) = 0;
    /// Add a scripted event handler by function for a specific sender.
    virtual void AddEventHandler(Object* sender, const String& eventName, int functionIndex) = 0;
    /// Add a scripted event handler by function name for a specific sender.
    virtual void AddEventHandler(Object* sender, const String& eventName, const String& functionName) = 0;
    /// Remove a scripted event handler.
    virtual void RemoveEventHandler(const String& eventName) = 0;
    /// Remove a scripted event handler for a specific sender.
    virtual void RemoveEventHandler(Object* sender, const String& eventName) = 0;
    /// Remove all scripted event handlers for a specific sender.
    virtual void RemoveEventHandlers(Object* sender) = 0;
    /// Remove all scripted event handlers.
    virtual void RemoveAllEventHandlers() = 0;
    /// Remove all scripted event handlers, except those listed.
    virtual void RemoveEventHandlersExcept(const Vector<String>& exceptionNames) = 0;
};

}
