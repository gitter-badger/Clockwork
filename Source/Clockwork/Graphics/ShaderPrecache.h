

#pragma once

#include "../Container/HashSet.h"
#include "../Core/Object.h"
#include "../Resource/XMLFile.h"

namespace Clockwork
{

class Graphics;
class ShaderVariation;

/// Utility class for collecting used shader combinations during runtime for precaching.
class CLOCKWORK_API ShaderPrecache : public Object
{
    OBJECT(ShaderPrecache);

public:
    /// Construct and begin collecting shader combinations. Load existing combinations from XML if the file exists.
    ShaderPrecache(Context* context, const String& fileName);
    /// Destruct. Write the collected shaders to XML.
    ~ShaderPrecache();

    /// Collect a shader combination. Called by Graphics when shaders have been set.
    void StoreShaders(ShaderVariation* vs, ShaderVariation* ps);

    /// Load shaders from an XML file.
    static void LoadShaders(Graphics* graphics, Deserializer& source);

private:
    /// XML file name.
    String fileName_;
    /// XML file.
    XMLFile xmlFile_;
    /// Already encountered shader combinations, pointer version for fast queries.
    HashSet<Pair<ShaderVariation*, ShaderVariation*> > usedPtrCombinations_;
    /// Already encountered shader combinations.
    HashSet<String> usedCombinations_;
};

}
