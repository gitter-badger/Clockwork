

#pragma once

#include "../Clockwork2D/Drawable2D.h"

namespace Clockwork
{

class Sprite2D;

/// Static sprite component.
class CLOCKWORK_API StaticSprite2D : public Drawable2D
{
    OBJECT(StaticSprite2D);

public:
    /// Construct.
    StaticSprite2D(Context* context);
    /// Destruct.
    ~StaticSprite2D();
    /// Register object factory. Drawable2D must be registered first.
    static void RegisterObject(Context* context);

    /// Set sprite.
    void SetSprite(Sprite2D* sprite);
    /// Set blend mode.
    void SetBlendMode(BlendMode blendMode);
    /// Set flip.
    void SetFlip(bool flipX, bool flipY);
    /// Set flip X.
    void SetFlipX(bool flipX);
    /// Set flip Y.
    void SetFlipY(bool flipY);
    /// Set color.
    void SetColor(const Color& color);
    /// Set alpha.
    void SetAlpha(float alpha);
    /// Set use hot spot.
    void SetUseHotSpot(bool useHotSpot);
    /// Set hot spot.
    void SetHotSpot(const Vector2& hotspot);
    /// Set custom material.
    void SetCustomMaterial(Material* customMaterial);

    /// Return sprite.
    Sprite2D* GetSprite() const;

    /// Return blend mode.
    BlendMode GetBlendMode() const { return blendMode_; }

    /// Return flip X.
    bool GetFlipX() const { return flipX_; }

    /// Return flip Y.
    bool GetFlipY() const { return flipY_; }

    /// Return color.
    const Color& GetColor() const { return color_; }

    /// Return alpha.
    float GetAlpha() const { return color_.a_; }

    /// Return use hot spot.
    bool GetUseHotSpot() const { return useHotSpot_; }

    /// Return hot spot.
    const Vector2& GetHotSpot() const { return hotSpot_; }

    /// Return custom material.
    Material* GetCustomMaterial() const;

    /// Set sprite attribute.
    void SetSpriteAttr(const ResourceRef& value);
    /// Return sprite attribute.
    ResourceRef GetSpriteAttr() const;
    /// Set custom material attribute.
    void SetCustomMaterialAttr(const ResourceRef& value);
    /// Return custom material attribute.
    ResourceRef GetCustomMaterialAttr() const;

protected:
    /// Recalculate the world-space bounding box.
    virtual void OnWorldBoundingBoxUpdate();
    /// Handle draw order changed.
    virtual void OnDrawOrderChanged();
    /// Update source batches.
    virtual void UpdateSourceBatches();
    /// Handle flip changed.
    virtual void OnFlipChanged();
    /// Update material.
    void UpdateMaterial();

    /// Sprite.
    SharedPtr<Sprite2D> sprite_;
    /// Blend mode.
    BlendMode blendMode_;
    /// Flip X.
    bool flipX_;
    /// Flip Y.
    bool flipY_;
    /// Color.
    Color color_;
    /// Use hot spot.
    bool useHotSpot_;
    /// Hot spot.
    Vector2 hotSpot_;
    /// Custom material.
    SharedPtr<Material> customMaterial_;
};

}
