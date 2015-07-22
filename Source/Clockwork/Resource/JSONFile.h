

#pragma once

#include "../Resource/Resource.h"
#include "../Resource/JSONValue.h"

namespace rapidjson
{

template <typename Encoding, typename Allocator> class GenericDocument;
typedef GenericDocument<UTF8<char>, MemoryPoolAllocator<CrtAllocator> > Document;

}

namespace Clockwork
{

/// JSON document resource.
class CLOCKWORK_API JSONFile : public Resource
{
    OBJECT(JSONFile);

public:
    /// Construct.
    JSONFile(Context* context);
    /// Destruct.
    virtual ~JSONFile();
    /// Register object factory.
    static void RegisterObject(Context* context);

    /// Load resource from stream. May be called from a worker thread. Return true if successful.
    virtual bool BeginLoad(Deserializer& source);
    /// Save resource with default indentation (one tab). Return true if successful.
    virtual bool Save(Serializer& dest) const;
    /// Save resource with user-defined indentation, only the first character (if any) of the string is used and the length of the string defines the character count. Return true if successful.
    bool Save(Serializer& dest, const String& indendation) const;

    /// Clear the document and create a root value, default is object type.
    JSONValue CreateRoot(JSONValueType valueType = JSON_OBJECT);
    /// Return the root value with specific value type, Return null value if not found.
    JSONValue GetRoot(JSONValueType valueType = JSON_ANY);

    /// Return rapidjson document.
    rapidjson::Document* GetDocument() const { return document_; }

private:
    /// Rapid JSON document.
    rapidjson::Document* document_;
};

}
