#pragma once

#include "../Core/Object.h"

namespace Clockwork
{

class LuaFunction;
class LuaScriptInstance;

/// Lua script event invoker.
class LuaScriptEventInvoker : public Object
{
    OBJECT(LuaScriptEventInvoker);

public:
    /// Construct.
    LuaScriptEventInvoker(Context* context);
    /// Construct from LuaScriptInstance.
    LuaScriptEventInvoker(LuaScriptInstance* instance);
    /// Destruct.
    virtual ~LuaScriptEventInvoker();

    /// Add a scripted event handler.
    void AddEventHandler(Object* sender, const StringHash& eventType, LuaFunction* function);

private:
    /// Handle script event in Lua script.
    void HandleLuaScriptEvent(StringHash eventType, VariantMap& eventData);

    /// Lua script instance.
    WeakPtr<LuaScriptInstance> instance_;
};

}
