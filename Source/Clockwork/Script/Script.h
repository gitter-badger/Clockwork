#pragma once

#include "../Core/Mutex.h"
#include "../Core/Object.h"

class asIObjectType;
class asIScriptContext;
class asIScriptEngine;
class asIScriptModule;

struct asSMessageInfo;

namespace Clockwork
{

extern const char* LOGIC_CATEGORY;

class Scene;
class ScriptFile;
class ScriptInstance;

/// Output mode for DumpAPI method.
enum DumpMode
{
    DOXYGEN = 0,
    C_HEADER,
    MAX_DUMP_MODES
};

/// Scripting subsystem. Allows execution of AngelScript.
class CLOCKWORK_API Script : public Object
{
    OBJECT(Script);

    friend class ScriptFile;

public:
    /// Construct.
    Script(Context* context);
    /// Destruct. Release the AngelScript engine.
    ~Script();

    /// Compile and execute a line of script in immediate mode.
    bool Execute(const String& line);
    /// Set immediate mode script file.
    void SetDefaultScriptFile(ScriptFile* file);
    /// Set immediate mode scene.
    void SetDefaultScene(Scene* scene);
    /// Set whether to execute engine console commands as script code.
    void SetExecuteConsoleCommands(bool enable);
    /// Print the whole script API (all registered classes, methods and properties) to the log. No-ops when CLOCKWORK_LOGGING not defined.
    void DumpAPI(DumpMode mode= DOXYGEN, const String& sourceTree = String::EMPTY);
    /// Log a message from the script engine.
    void MessageCallback(const asSMessageInfo* msg);
    /// Handle a script exception.
    void ExceptionCallback(asIScriptContext* context);
    /// Get call stack.
    static String GetCallStack(asIScriptContext* context);

    /// Return the AngelScript engine.
    asIScriptEngine* GetScriptEngine() const { return scriptEngine_; }
    /// Return immediate execution script context.
    asIScriptContext* GetImmediateContext() const { return immediateContext_; }
    /// Return immediate mode script file.
    ScriptFile* GetDefaultScriptFile() const;
    /// Return immediate mode scene.
    Scene* GetDefaultScene() const;
    /// Return whether is executing engine console commands as script code.
    bool GetExecuteConsoleCommands() const { return executeConsoleCommands_; }
    /// Clear the inbuild object type cache.
    void ClearObjectTypeCache();
    /// Query for an inbuilt object type by constant declaration. Can not be used for script types.
    asIObjectType* GetObjectType(const char* declaration);
    /// Return the script module create/delete mutex.
    Mutex& GetModuleMutex() { return moduleMutex_; }

private:
    /// Increase script nesting level.
    void IncScriptNestingLevel() { ++scriptNestingLevel_; }
    /// Decrease script nesting level.
    void DecScriptNestingLevel() { --scriptNestingLevel_; }
    /// Return current script nesting level.
    unsigned GetScriptNestingLevel() { return scriptNestingLevel_; }
    /// Return a script function/method execution context for the current execution nesting level.
    asIScriptContext* GetScriptFileContext();
    /// Output a sanitated row of script API. No-ops when CLOCKWORK_LOGGING not defined.
    void OutputAPIRow(DumpMode mode, const String& row, bool removeReference = false, String separator = ";");
    /// Handle a console command event.
    void HandleConsoleCommand(StringHash eventType, VariantMap& eventData);

    /// AngelScript engine.
    asIScriptEngine* scriptEngine_;
    /// Immediate execution script context.
    asIScriptContext* immediateContext_;
    /// Immediate execution script file.
    WeakPtr<ScriptFile> defaultScriptFile_;
    /// Immediate execution scene.
    WeakPtr<Scene> defaultScene_;
    /// Script function/method execution contexts.
    Vector<asIScriptContext*> scriptFileContexts_;
    /// Search cache for inbuilt object types.
    HashMap<const char*, asIObjectType*> objectTypes_;
    /// Script module create/delete mutex.
    Mutex moduleMutex_;
    /// Current script execution nesting level.
    unsigned scriptNestingLevel_;
    /// Flag for executing engine console commands as script code. Default to true.
    bool executeConsoleCommands_;
};

/// Register Script library objects.
void CLOCKWORK_API RegisterScriptLibrary(Context* context);

}
