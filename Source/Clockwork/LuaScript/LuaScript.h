#pragma once

#include "../Core/Context.h"
#include "../Core/Object.h"
#include "../LuaScript/LuaScriptEventListener.h"

struct lua_State;

namespace Clockwork
{

extern const char* LOGIC_CATEGORY;

class LuaFunction;
class LuaScriptEventInvoker;
class Scene;

/// Lua script subsystem.
class CLOCKWORK_API LuaScript : public Object, public LuaScriptEventListener
{
    OBJECT(LuaScript);

public:
    /// Construct.
    LuaScript(Context* context);
    /// Destruct.
    ~LuaScript();

    /// Add a scripted event handler by function.
    virtual void AddEventHandler(const String& eventName, int functionIndex);
    /// Add a scripted event handler by function name.
    virtual void AddEventHandler(const String& eventName, const String& functionName);
    /// Add a scripted event handler by function for a specific sender.
    virtual void AddEventHandler(Object* sender, const String& eventName, int functionIndex);
    /// Add a scripted event handler by function name for a specific sender.
    virtual void AddEventHandler(Object* sender, const String& eventName, const String& functionName);
    /// Remove a scripted event handler.
    virtual void RemoveEventHandler(const String& eventName);
    /// Remove a scripted event handler for a specific sender.
    virtual void RemoveEventHandler(Object* sender, const String& eventName);
    /// Remove all scripted event handlers for a specific sender.
    virtual void RemoveEventHandlers(Object* sender);
    /// Remove all scripted event handlers.
    virtual void RemoveAllEventHandlers();
    /// Remove all scripted event handlers, except those listed.
    virtual void RemoveEventHandlersExcept(const Vector<String>& exceptionNames);

    /// Execute script file. Return true if successful.
    bool ExecuteFile(const String& fileName);
    /// Execute script string. Return true if successful.
    bool ExecuteString(const String& string);
    /// Execute script function.
    bool ExecuteFunction(const String& functionName);
    /// Send event.
    void SendEvent(const String& eventName, VariantMap& eventData);
    /// Set whether to execute engine console commands as script code.
    void SetExecuteConsoleCommands(bool enable);

    /// Return Lua state.
    lua_State* GetState() const { return luaState_; }
    /// Return Lua function by function stack index.
    LuaFunction* GetFunction(int functionIndex);
    /// Return Lua function by function name.
    LuaFunction* GetFunction(const String& functionName, bool silentIfNotfound = false);
    /// Return whether is executing engine console commands as script code.
    bool GetExecuteConsoleCommands() const { return executeConsoleCommands_; }

private:
    /// Register loader.
    void RegisterLoader();
    /// Replace print.
    void ReplacePrint();
    /// Handle post update.
    void HandlePostUpdate(StringHash eventType, VariantMap& eventData);
    /// Handle a console command event.
    void HandleConsoleCommand(StringHash eventType, VariantMap& eventData);
    /// Push script function.
    bool PushScriptFunction(const String& functionName, bool silentIfNotfound = false);

    /// At panic.
    static int AtPanic(lua_State* L);
    /// Loader.
    static int Loader(lua_State* L);
    /// Print function.
    static int Print(lua_State* L);

    /// Lua state.
    lua_State* luaState_;
    /// Event invoker.
    SharedPtr<LuaScriptEventInvoker> eventInvoker_;
    /// Coroutine update function.
    LuaFunction* coroutineUpdate_;
    /// Flag for executing engine console commands as script code. Default to true.
    bool executeConsoleCommands_;
    /// Function pointer to function map.
    HashMap<const void*, SharedPtr<LuaFunction> > functionPointerToFunctionMap_;
    /// Function name to function map.
    HashMap<String, SharedPtr<LuaFunction> > functionNameToFunctionMap_;
};

/// Register Lua script library objects.
void CLOCKWORK_API RegisterLuaScriptLibrary(Context* context);

}
