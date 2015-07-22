

#pragma once

#include "../Container/ArrayPtr.h"
#include "../Resource/Resource.h"

struct lua_State;

namespace Clockwork
{

/// Lua file.
class CLOCKWORK_API LuaFile : public Resource
{
    OBJECT(LuaFile);

public:
    /// Construct.
    LuaFile(Context* context);
    /// Destruct.
    virtual ~LuaFile();
    /// Register object factory.
    static void RegisterObject(Context* context);

    /// Load resource from stream. May be called from a worker thread. Return true if successful.
    virtual bool BeginLoad(Deserializer& source);
    /// Save resource. Return true if successful.
    virtual bool Save(Serializer& dest) const;

    /// Load buffer as chunk.
    bool LoadChunk(lua_State* luaState);
    /// Load buffer as chunk and execute.
    bool LoadAndExecute(lua_State* luaState);

private:
    /// File size.
    unsigned size_;
    /// File data.
    SharedArrayPtr<char> data_;
    /// Has loaded.
    bool hasLoaded_;
    /// Has executed.
    bool hasExecuted_;
};

}
