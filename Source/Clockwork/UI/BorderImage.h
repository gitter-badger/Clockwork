

#pragma once

#include "../Graphics/GraphicsDefs.h"
#include "../UI/UIElement.h"

namespace Clockwork
{

class Texture;
class Texture2D;

/// %Image %UI element with optional border.
class CLOCKWORK_API BorderImage : public UIElement
{
    OBJECT(BorderImage);

public:
    /// Construct.
    BorderImage(Context* context);
    /// Destruct.
    virtual ~BorderImage();
    /// Register object factory.
    static void RegisterObject(Context* context);

    /// Return UI rendering batches.
    virtual void GetBatches(PODVector<UIBatch>& batches, PODVector<float>& vertexData, const IntRect& currentScissor);

    /// Set texture.
    void SetTexture(Texture* texture);
    /// Set part of texture to use as the image.
    void SetImageRect(const IntRect& rect);
    /// Use whole texture as the image.
    void SetFullImageRect();
    /// Set border dimensions on the screen.
    void SetBorder(const IntRect& rect);
    /// Set border dimensions on the image. If zero (default) uses the screen dimensions, resulting in pixel-perfect borders.
    void SetImageBorder(const IntRect& rect);
    /// Set offset to image rectangle used on hover.
    void SetHoverOffset(const IntVector2& offset);
    /// Set offset to image rectangle used on hover.
    void SetHoverOffset(int x, int y);
    /// Set blend mode.
    void SetBlendMode(BlendMode mode);
    /// Set tiled mode.
    void SetTiled(bool enable);

    /// Return texture.
    Texture* GetTexture() const { return texture_; }

    /// Return image rectangle.
    const IntRect& GetImageRect() const { return imageRect_; }

    /// Return border screen dimensions.
    const IntRect& GetBorder() const { return border_; }

    /// Return border image dimensions. Zero rect uses border screen dimensions.
    const IntRect& GetImageBorder() const { return imageBorder_; }

    /// Return offset to image rectangle used on hover.
    const IntVector2& GetHoverOffset() const { return hoverOffset_; }

    /// Return blend mode.
    BlendMode GetBlendMode() const { return blendMode_; }

    /// Return whether is tiled.
    bool IsTiled() const { return tiled_; }

    /// Set texture attribute.
    void SetTextureAttr(const ResourceRef& value);
    /// Return texture attribute.
    ResourceRef GetTextureAttr() const;

protected:
    /// Return UI rendering batches with offset to image rectangle.
    void GetBatches
        (PODVector<UIBatch>& batches, PODVector<float>& vertexData, const IntRect& currentScissor, const IntVector2& offset);

    /// Texture.
    SharedPtr<Texture> texture_;
    /// Image rectangle.
    IntRect imageRect_;
    /// Border dimensions on screen.
    IntRect border_;
    /// Border dimensions on the image.
    IntRect imageBorder_;
    /// Offset to image rectangle on hover.
    IntVector2 hoverOffset_;
    /// Blend mode flag.
    BlendMode blendMode_;
    /// Tiled flag.
    bool tiled_;
};

}
