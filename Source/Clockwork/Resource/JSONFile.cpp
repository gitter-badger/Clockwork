#include "../Container/ArrayPtr.h"
#include "../Core/Context.h"
#include "../IO/Deserializer.h"
#include "../Resource/JSONFile.h"
#include "../IO/Log.h"
#include "../Core/Profiler.h"
#include "../Resource/ResourceCache.h"
#include "../IO/Serializer.h"
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>

#include "../DebugNew.h"

using namespace rapidjson;

namespace Clockwork
{

JSONFile::JSONFile(Context* context) :
    Resource(context),
    document_(new Document())
{
}

JSONFile::~JSONFile()
{
    delete document_;
    document_ = 0;
}

void JSONFile::RegisterObject(Context* context)
{
    context->RegisterFactory<JSONFile>();
}

bool JSONFile::BeginLoad(Deserializer& source)
{
    unsigned dataSize = source.GetSize();
    if (!dataSize && !source.GetName().Empty())
    {
        LOGERROR("Zero sized JSON data in " + source.GetName());
        return false;
    }

    SharedArrayPtr<char> buffer(new char[dataSize + 1]);
    if (source.Read(buffer.Get(), dataSize) != dataSize)
        return false;
    buffer[dataSize] = '\0';

    if (document_->Parse<0>(buffer).HasParseError())
    {
        LOGERROR("Could not parse JSON data from " + source.GetName());
        return false;
    }

    SetMemoryUse(dataSize);

    return true;
}

bool JSONFile::Save(Serializer& dest) const
{
    return Save(dest, "\t");
}

bool JSONFile::Save(Serializer& dest, const String& indendation) const
{
    StringBuffer buffer;
    PrettyWriter<StringBuffer> writer(buffer, &(document_->GetAllocator()));
    writer.SetIndent(!indendation.Empty() ?  indendation.Front() : '\0', indendation.Length());

    document_->Accept(writer);
    unsigned size = (unsigned)buffer.GetSize();
    return dest.Write(buffer.GetString(), size) == size;
}

JSONValue JSONFile::CreateRoot(JSONValueType valueType)
{
    if (valueType == JSON_OBJECT)
        document_->SetObject();
    else
        document_->SetArray();

    return JSONValue(this, document_);
}

JSONValue JSONFile::GetRoot(JSONValueType valueType)
{
    if (!document_)
        return JSONValue::EMPTY;

    if ((valueType == JSON_OBJECT && document_->GetType() != kObjectType) ||
        (valueType == JSON_ARRAY && document_->GetType() != kArrayType))
    {
        LOGERROR("Invalid root value type");
        return JSONValue::EMPTY;
    }

    return JSONValue(this, document_);
}

}
