

#pragma once

#include "../Core/Context.h"
#include "../Graphics/OctreeQuery.h"
#include "../Math/Vector2.h"
#include "../Math/Vector3.h"
#ifdef CLOCKWORK_PHYSICS
#include "../Physics/PhysicsWorld.h"
#endif
#ifdef CLOCKWORK_CLOCKWORK2D
#include "../Clockwork2D/PhysicsWorld2D.h"
#endif

struct lua_State;

namespace Clockwork
{
class Component;
class Context;
#ifdef CLOCKWORK_NAVIGATION
class CrowdAgent;
#endif
class Pass;
class SoundSource;
class UIElement;
}

using namespace Clockwork;

/// Check is String.
#define tolua_isclockwork3dstring tolua_isstring
/// Push String.
#define tolua_pushclockwork3dstring(x, y) tolua_pushstring(x, y.CString())
/// Convert to String.
const char* tolua_toclockwork3dstring(lua_State* L, int narg, const char* str);
/// Convert to String.
const char* tolua_toclockwork3dstring(lua_State* L, int narg, const String& str);

/// Set context.
void SetContext(lua_State* L, Context* context);
/// Return context.
Context* GetContext(lua_State* L);

/// Create object.
template <typename T> int ToluaNewObject(lua_State* tolua_S)
{
    T* object = Mtolua_new(T(GetContext(tolua_S)));
    tolua_pushusertype(tolua_S, (void*)object, T::GetTypeNameStatic().CString());
    return 1;
}

/// Create object with garbage collection.
template <typename T> int ToluaNewObjectGC(lua_State* tolua_S)
{
    T* object = Mtolua_new(T(GetContext(tolua_S)));
    tolua_pushusertype(tolua_S, (void*)object, T::GetTypeNameStatic().CString());
    tolua_register_gc(tolua_S, lua_gettop(tolua_S));
    return 1;
}

/// Return subsystem.
template <typename T> int ToluaGetSubsystem(lua_State* tolua_S)
{
    T* subsystem = GetContext(tolua_S)->GetSubsystem<T>();
    tolua_pushusertype(tolua_S, (void*)subsystem, T::GetTypeNameStatic().CString());
    return 1;
}

/// Check is Vector<T>.
template <typename T> int ToluaIsVector(lua_State* L, int lo, const char* type, int def, tolua_Error* err);
/// Check is Vector<String>.
template <> int ToluaIsVector<String>(lua_State* L, int lo, const char* type, int def, tolua_Error* err);

/// Convert to Vector<T>.
template <typename T> void* ToluaToVector(lua_State* L, int narg, void* def);
/// Convert to Vector<String>.
template <> void* ToluaToVector<String>(lua_State* L, int narg, void* def);

/// Push Vector<T> to Lua as a table.
template <typename T> int ToluaPushVector(lua_State* L, void* data, const char* type);
/// Push Vector<String> to Lua as a table.
template <> int ToluaPushVector<String>(lua_State* L, void* data, const char* type);
/// Push Vector<StringHash> to Lua as a table.
template <> int ToluaPushVector<StringHash>(lua_State* L, void* data, const char* type);

/// Check is PODVector<T>.
template <typename T> int ToluaIsPODVector(lua_State* L, int lo, const char* type, int def, tolua_Error* err);
/// Check is PODVector<unsigned>.
template <> int ToluaIsPODVector<unsigned>(lua_State* L, int lo, const char* type, int def, tolua_Error* err);
/// Check is PODVector<Vector2>.
template <> int ToluaIsPODVector<Vector2>(lua_State* L, int lo, const char* type, int def, tolua_Error* err);

/// Convert to PODVector<T>.
template <typename T> void* ToluaToPODVector(lua_State* L, int narg, void* def);
/// Convert PODVector<unsigned>.
template <> void* ToluaToPODVector<unsigned>(lua_State* L, int narg, void* def);
/// Convert PODVector<Vector2>.
template <> void* ToluaToPODVector<Vector2>(lua_State* L, int narg, void* def);

/// Push PODVector<T> to Lua as a table.
template <typename T> int ToluaPushPODVector(lua_State* L, void* data, const char* type);
/// Push PODVector<int> to Lua as a table.
template <> int ToluaPushPODVector<int>(lua_State* L, void* data, const char* type);
/// Push PODVector<unsigned> to Lua as a table.
template <> int ToluaPushPODVector<unsigned>(lua_State* L, void* data, const char* type);

/// Push PODVector<Component*> to Lua as a table.
template <> int ToluaPushPODVector<Component*>(lua_State* L, void* data, const char* type);

/// Push PODVector<Node*> to Lua as a table.
template <> int ToluaPushPODVector<Node*>(lua_State* L, void* data, const char* type);

/// Push PODVector<SoundSource*> to Lua as a table.
template <> int ToluaPushPODVector<SoundSource*>(lua_State* L, void* data, const char* type);
/// Push PODVector<UIElement*> to Lua as a table.
template <> int ToluaPushPODVector<UIElement*>(lua_State* L, void* data, const char* type);
#ifdef CLOCKWORK_PHYSICS
/// Push PODVector<RigidBody*> to Lua as a table.
template <> int ToluaPushPODVector<RigidBody*>(lua_State* L, void* data, const char* type);
#endif
#ifdef CLOCKWORK_CLOCKWORK2D
/// Push PODVector<RigidBody2D*> to Lua as a table.
template <> int ToluaPushPODVector<RigidBody2D*>(lua_State* L, void* data, const char* type);
#endif
/// Push PODVector<Vector3> to Lua as a table.
template <> int ToluaPushPODVector<Vector3>(lua_State* L, void* data, const char* type);
/// Push PODVector<IntVector2> to Lua as a table.
template <> int ToluaPushPODVector<IntVector2>(lua_State* L, void* data, const char* type);
/// Push PODVector<OctreeQueryResult> to Lua as a table.
template <> int ToluaPushPODVector<OctreeQueryResult>(lua_State* L, void* data, const char* type);
#ifdef CLOCKWORK_NAVIGATION
/// Push PODVector<CrowdAgent*> to Lua as a table.
template <> int ToluaPushPODVector<CrowdAgent*>(lua_State* L, void* data, const char* type);
#endif
#ifdef CLOCKWORK_PHYSICS
/// Push PODVector<PhysicsRaycastResult> to Lua as a table.
template <> int ToluaPushPODVector<PhysicsRaycastResult>(lua_State* L, void* data, const char* type);
#endif
#ifdef CLOCKWORK_CLOCKWORK2D
/// Push PODVector<PhysicsRaycastResult2D> to Lua as a table.
template <> int ToluaPushPODVector<PhysicsRaycastResult2D>(lua_State* L, void* data, const char* type);
#endif
/// Push PODVector<RayQueryResult> to Lua as a table.
template <> int ToluaPushPODVector<RayQueryResult>(lua_State* L, void* data, const char* type);
/// Push PODVector<Pass*> to Lua as a table.
template <> int ToluaPushPODVector<Pass*>(lua_State* L, void* data, const char* type);
/// Push Object to Lua.
void ToluaPushObject(lua_State* L, void* data, const char* type);

