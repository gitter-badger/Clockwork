

#pragma once

#include "../Container/RefCounted.h"

struct lua_State;

namespace Clockwork
{

class LuaScript;
class LuaScriptInstance;
class Variant;

/// Lua function.
class CLOCKWORK_API LuaFunction : public RefCounted
{
public:
    /// Construct.
    LuaFunction(lua_State* lusState, int functionRef, bool needUnref = true);
    /// Destruct.
    ~LuaFunction();

    /// Check that function is valid.
    bool IsValid() const;
    /// Begin function call.
    bool BeginCall();
    /// Begin script object's function call.
    bool BeginCall(const LuaScriptInstance* instance);
    /// End call and actually execute the function.
    bool EndCall(int numReturns = 0);
    /// Push int to stack.
    void PushInt(int value);
    /// Push bool to stack.
    void PushBool(bool value);
    /// Push float to stack.
    void PushFloat(float value);
    /// Push string to stack.
    void PushString(const String& string);
    /// Push user type to stack.
    void PushUserType(void* userType, const char* typeName);

    /// Push user type to stack.
    template <typename T> void PushUserType(const T* userType)
    {
        PushUserType((void*)userType, T::GetTypeName().CString());
    }

    /// Push user type to stack.
    template <typename T> void PushUserType(const T& userType)
    {
        PushUserType((void*)&userType, T::GetTypeName().CString());
    }

    /// Push user type to stack.
    template <typename T> void PushUserType(const T* userType, const char* typeName)
    {
        PushUserType((void*)userType, typeName);
    }

    /// Push user type to stack.
    template <typename T> void PushUserType(const T& userType, const char* typeName)
    {
        PushUserType((void*)&userType, typeName);
    }

    /// Push variant to stack.
    bool PushVariant(const Variant& variant);
    /// Push Lua table to stack.
    bool PushLuaTable(const String& tableName);

    /// Return function ref.
    int GetFunctionRef() const { return functionRef_; }

private:
    /// Lua state.
    lua_State* luaState_;
    /// Function ref.
    int functionRef_;
    /// Need unref.
    bool needUnref_;
    /// Lua stack top.
    int stackTop_;
    /// Number of arguments.
    int numArguments_;
};

}
