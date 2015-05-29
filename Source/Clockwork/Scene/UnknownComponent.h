#pragma once

#include "../Scene/Component.h"

namespace Clockwork
{

/// Placeholder for allowing unregistered components to be loaded & saved along with scenes.
class CLOCKWORK_API UnknownComponent : public Component
{
public:
    /// Construct.
    UnknownComponent(Context* context);

    /// Register object factory.
    static void RegisterObject(Context* context);

    /// Return type of the stored component.
    virtual StringHash GetType() const { return typeHash_; }
    /// Return type name of the stored component.
    virtual const String& GetTypeName() const { return typeName_; }
    /// Return attribute descriptions, or null if none defined.
    virtual const Vector<AttributeInfo>* GetAttributes() const { return &xmlAttributeInfos_; }
    /// Load from binary data. Return true if successful.
    virtual bool Load(Deserializer& source, bool setInstanceDefault = false);
    /// Load from XML data. Return true if successful.
    virtual bool LoadXML(const XMLElement& source, bool setInstanceDefault = false);
    /// Save as binary data. Return true if successful.
    virtual bool Save(Serializer& dest) const;
    /// Save as XML data. Return true if successful.
    virtual bool SaveXML(XMLElement& dest) const;

    /// Initialize the type name. Called by Node when loading.
    void SetTypeName(const String& typeName);
   /// Initialize the type hash only when type name not known. Called by Node when loading.
    void SetType(StringHash typeHash);

    /// Return the XML format attributes. Empty when loaded with binary serialization.
    const Vector<String>& GetXMLAttributes() const { return xmlAttributes_; }
    /// Return the binary attributes. Empty when loaded with XML serialization.
    const PODVector<unsigned char>& GetBinaryAttributes() const { return binaryAttributes_; }
    /// Return whether was loaded using XML data.
    bool GetUseXML() const { return useXML_; }

    /// Return static type.
    static Clockwork::StringHash GetTypeStatic() { static const StringHash typeStatic("UnknownComponent"); return typeStatic; } \
    /// Return static type name.
    static const Clockwork::String& GetTypeNameStatic() { static const String typeNameStatic("UnknownComponent"); return typeNameStatic; } \

private:
    /// Type of stored component.
    StringHash typeHash_;
    /// Type name of the stored component.
    String typeName_;
    /// XML format attribute infos.
    Vector<AttributeInfo> xmlAttributeInfos_;
    /// XML format attribute data (as strings)
    Vector<String> xmlAttributes_;
    /// Binary attributes.
    PODVector<unsigned char> binaryAttributes_;
    /// Flag of whether was loaded using XML data.
    bool useXML_;
};

}
