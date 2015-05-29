#pragma once

#include "../Resource/Resource.h"

namespace Clockwork
{

class SpriteSheet2D;
class Texture2D;

/// Sprite.
class CLOCKWORK_API Sprite2D : public Resource
{
    OBJECT(Sprite2D);

public:
    /// Construct.
    Sprite2D(Context* context);
    /// Destruct.
    virtual ~Sprite2D();
    /// Register object factory.
    static void RegisterObject(Context* context);

    /// Load resource from stream. May be called from a worker thread. Return true if successful.
    virtual bool BeginLoad(Deserializer& source);
    /// Finish resource loading. Always called from the main thread. Return true if successful.
    virtual bool EndLoad();

    /// Set texture.
    void SetTexture(Texture2D* texture);
    /// Set rectangle.
    void SetRectangle(const IntRect& rectangle);
    /// Set hot spot.
    void SetHotSpot(const Vector2& hotSpot);
    /// Set offset.
    void SetOffset(const IntVector2& offset);
    /// Set sprite sheet.
    void SetSpriteSheet(SpriteSheet2D* spriteSheet);

    /// Return texture.
    Texture2D* GetTexture() const { return texture_; }
    /// Return rectangle.
    const IntRect& GetRectangle() const { return rectangle_; }
    /// Return hot spot.
    const Vector2& GetHotSpot() const { return hotSpot_; }
    /// Return offset.
    const IntVector2& GetOffset() const { return offset_; }
    /// Return sprite sheet.
    SpriteSheet2D* GetSpriteSheet() const { return spriteSheet_; }

    /// Return draw rectangle.
    bool GetDrawRectangle(Rect& rect, bool flipX = false, bool flipY = false) const;
    /// Return draw rectangle with custom hot spot.
    bool GetDrawRectangle(Rect& rect, const Vector2& hotSpot, bool flipX = false, bool flipY = false) const;
    /// Return texture rectangle.
    bool GetTextureRectangle(Rect& rect, bool flipX = false, bool flipY = false) const;

    /// Save sprite to ResourceRef.
    static ResourceRef SaveToResourceRef(Sprite2D* sprite);
    /// Load sprite from ResourceRef.
    static Sprite2D* LoadFromResourceRef(Object* object, const ResourceRef& value);

private:
    /// Texture.
    SharedPtr<Texture2D> texture_;
    /// Rectangle.
    IntRect rectangle_;
    /// Hot spot.
    Vector2 hotSpot_;
    /// Offset (for trimmed sprite).
    IntVector2 offset_;
    /// Sprite sheet.
    WeakPtr<SpriteSheet2D> spriteSheet_;
    /// Texture used while loading.
    SharedPtr<Texture2D> loadTexture_;
};

}
