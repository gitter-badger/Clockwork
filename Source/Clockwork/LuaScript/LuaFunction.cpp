#include "../IO/Log.h"
#include "../LuaScript/LuaFunction.h"
#include "../LuaScript/LuaScript.h"
#include "../LuaScript/LuaScriptInstance.h"
#include "../IO/VectorBuffer.h"

#include <toluapp/tolua++.h>
#include "../LuaScript/ToluaUtils.h"

#include "../DebugNew.h"

namespace Clockwork
{

LuaFunction::LuaFunction(lua_State* luaState, int functionRef, bool needUnref) :
    luaState_(luaState),
    functionRef_(functionRef),
    needUnref_(needUnref)
{
}

LuaFunction::~LuaFunction()
{
    if (needUnref_ && functionRef_ != LUA_REFNIL)
        luaL_unref(luaState_, LUA_REGISTRYINDEX, functionRef_);
}

bool LuaFunction::IsValid() const
{
    return functionRef_ != LUA_REFNIL;
}

bool LuaFunction::BeginCall()
{
    if (!IsValid())
        return false;

    stackTop_ = lua_gettop(luaState_);
    numArguments_ = 0;
    lua_rawgeti(luaState_, LUA_REGISTRYINDEX, functionRef_);

    return true;
}

bool LuaFunction::BeginCall(const LuaScriptInstance* instance)
{
    if (!IsValid())
        return false;

    stackTop_ = lua_gettop(luaState_);
    numArguments_ = 1;
    lua_rawgeti(luaState_, LUA_REGISTRYINDEX, functionRef_);
    lua_rawgeti(luaState_, LUA_REGISTRYINDEX, instance->GetScriptObjectRef());

    return true;
}

bool LuaFunction::EndCall(int numReturns)
{
    if (lua_pcall(luaState_, numArguments_, numReturns, 0) != 0)
    {
        const char* message = lua_tostring(luaState_, -1);
        LOGERROR("Execute Lua function failed: " + String(message));
        lua_settop(luaState_, stackTop_);
        return false;
    }

    return true;
}

void LuaFunction::PushInt(int value)
{
    ++numArguments_;

    lua_pushinteger(luaState_, value);
}

void LuaFunction::PushBool(bool value)
{
    ++numArguments_;

    lua_pushboolean(luaState_, value);
}

void LuaFunction::PushFloat(float value)
{
    ++numArguments_;

    lua_pushnumber(luaState_, value);
}

void LuaFunction::PushString(const String& string)
{
    ++numArguments_;

    tolua_pushclockwork3dstring(luaState_, string);
}

void LuaFunction::PushUserType(void* userType, const char* typeName)
{
    ++numArguments_;

    tolua_pushusertype(luaState_, userType, typeName);
}

bool LuaFunction::PushVariant(const Variant& variant)
{
    switch (variant.GetType())
    {
    case VAR_INT:
        PushInt(variant.GetInt());
        return true;

    case VAR_BOOL:
        PushBool(variant.GetBool());
        return true;

    case VAR_FLOAT:
        PushFloat(variant.GetFloat());
        return true;

    case VAR_VECTOR2:
        PushUserType(variant.GetVector2(), "Vector2");
        return true;

    case VAR_VECTOR3:
        PushUserType(variant.GetVector3(), "Vector3");
        return true;

    case VAR_VECTOR4:
        PushUserType(variant.GetVector4(), "Vector4");
        return true;

    case VAR_QUATERNION:
        PushUserType(variant.GetQuaternion(), "Quaternion");
        return true;

    case VAR_COLOR:
        PushUserType(variant.GetColor(), "Color");
        return true;

    case VAR_STRING:
        PushString(variant.GetString());
        return true;

    case VAR_BUFFER:
        {
            VectorBuffer buffer(variant.GetBuffer());
            PushUserType(buffer, "VectorBuffer");
        }
        return true;

    case VAR_RESOURCEREF:
        PushUserType(variant.GetResourceRef(), "ResourceRef");
        return true;

    case VAR_INTRECT:
        PushUserType(variant.GetIntRect(), "IntRect");
        return true;

    case VAR_INTVECTOR2:
        PushUserType(variant.GetIntVector2(), "IntVector2");
        return true;

    default:
        return false;
    }
}

bool LuaFunction::PushLuaTable(const String& tableName)
{
    ++numArguments_;

    lua_getglobal(luaState_, tableName.CString());
    if (!lua_istable(luaState_, -1))
    {
        LOGERROR("Could not find lua table " + tableName);
        return false;
    }
    return true;
}

}
