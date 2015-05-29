#pragma once

#include "../Resource/Resource.h"
#include "../Resource/XMLElement.h"

namespace pugi
{
    class xml_document;
    class xml_node;
    class xpath_node;
}

namespace Clockwork
{

/// XML document resource.
class CLOCKWORK_API XMLFile : public Resource
{
    OBJECT(XMLFile);

public:
    /// Construct.
    XMLFile(Context* context);
    /// Destruct.
    virtual ~XMLFile();
    /// Register object factory.
    static void RegisterObject(Context* context);

    /// Load resource from stream. May be called from a worker thread. Return true if successful.
    virtual bool BeginLoad(Deserializer& source);
    /// Save resource with default indentation (one tab). Return true if successful.
    virtual bool Save(Serializer& dest) const;
    /// Save resource with user-defined indentation. Return true if successful.
    bool Save(Serializer& dest, const String& indendation) const;

    /// Deserialize from a string. Return true if successful.
    bool FromString(const String& source);
    /// Clear the document and create a root element.
    XMLElement CreateRoot(const String& name);

    /// Return the root element, with optionally specified name. Return null element if not found.
    XMLElement GetRoot(const String& name = String::EMPTY);
    /// Return the pugixml document.
    pugi::xml_document* GetDocument() const { return document_; }
    /// Serialize the XML content to a string.
    String ToString(const String& indendation = "\t") const;

    /// Patch the XMLFile with another XMLFile. Based on RFC 5261.
    void Patch(XMLFile* patchFile);
    /// Patch the XMLFile with another XMLElement. Based on RFC 5261.
    void Patch(XMLElement patchElement);

private:
    /// Add an node in the Patch.
    void PatchAdd(const pugi::xml_node& patch, pugi::xpath_node& original);
    /// Replace a node or attribute in the Patch.
    void PatchReplace(const pugi::xml_node& patch, pugi::xpath_node& original);
    /// Remove a node or attribute in the Patch.
    void PatchRemove(const pugi::xpath_node& original);

    /// Add a node in the Patch.
    void AddNode(const pugi::xml_node& patch, pugi::xpath_node& original);
    /// Add an attribute in the Patch.
    void AddAttribute(const pugi::xml_node& patch, pugi::xpath_node& original);
    /// Combine two text nodes.
    bool CombineText(const pugi::xml_node& patch, pugi::xml_node original, bool prepend);

    /// Pugixml document.
    pugi::xml_document* document_;
};

}
