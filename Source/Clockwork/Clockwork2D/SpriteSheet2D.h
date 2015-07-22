

#pragma once

#include "../Resource/Resource.h"

namespace Clockwork
{

class PListFile;
class Sprite2D;
class Texture2D;
class XMLFile;

/// Sprite sheet.
class CLOCKWORK_API SpriteSheet2D : public Resource
{
    OBJECT(SpriteSheet2D);

public:
    /// Construct.
    SpriteSheet2D(Context* context);
    /// Destruct.
    virtual ~SpriteSheet2D();
    /// Register object factory.
    static void RegisterObject(Context* context);

    /// Load resource from stream. May be called from a worker thread. Return true if successful.
    virtual bool BeginLoad(Deserializer& source);
    /// Finish resource loading. Always called from the main thread. Return true if successful.
    virtual bool EndLoad();

    /// Return texture.
    Texture2D* GetTexture() const { return texture_; }

    /// Return sprite.
    Sprite2D* GetSprite(const String& name) const;
    /// Define sprite.
    void DefineSprite(const String& name, const IntRect& rectangle, const Vector2& hotSpot = Vector2(0.5f, 0.5f),
        const IntVector2& offset = IntVector2::ZERO);

    /// Return sprite mapping.
    const HashMap<String, SharedPtr<Sprite2D> >& GetSpriteMapping() const { return spriteMapping_; }

private:
    /// Begin load from PList file.
    bool BeginLoadFromPListFile(Deserializer& source);
    /// End load from PList file.
    bool EndLoadFromPListFile();
    /// Begin load from XML file.
    bool BeginLoadFromXMLFile(Deserializer& source);
    /// End load from XML file.
    bool EndLoadFromXMLFile();

    /// Texture.
    SharedPtr<Texture2D> texture_;
    /// Sprite mapping.
    HashMap<String, SharedPtr<Sprite2D> > spriteMapping_;
    /// PList file used while loading.
    SharedPtr<PListFile> loadPListFile_;
    /// XML file used while loading.
    SharedPtr<XMLFile> loadXMLFile_;
    /// Texture name used while loading.
    String loadTextureName_;
};

}
