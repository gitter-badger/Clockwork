

#pragma once

#include "../Math/Color.h"
#include "../Math/Rect.h"
#include "../Graphics/GraphicsDefs.h"

namespace Clockwork
{

class PixelShader;
class Graphics;
class Matrix3x4;
class Texture;
class UIElement;

static const unsigned UI_VERTEX_SIZE = 6;

/// %UI rendering draw call.
class CLOCKWORK_API UIBatch
{
public:
    /// Construct with defaults.
    UIBatch();
    /// Construct.
    UIBatch(UIElement* element, BlendMode blendMode, const IntRect& scissor, Texture* texture, PODVector<float>* vertexData);

    /// Set new color for the batch. Overrides gradient.
    void SetColor(const Color& color, bool overrideAlpha = false);
    /// Restore UI element's default color.
    void SetDefaultColor();
    /// Add a quad.
    void AddQuad(int x, int y, int width, int height, int texOffsetX, int texOffsetY, int texWidth = 0, int texHeight = 0);
    /// Add a quad using a transform matrix.
    void AddQuad(const Matrix3x4& transform, int x, int y, int width, int height, int texOffsetX, int texOffsetY, int texWidth = 0,
        int texHeight = 0);
    /// Add a quad with tiled texture.
    void AddQuad(int x, int y, int width, int height, int texOffsetX, int texOffsetY, int texWidth, int texHeight, bool tiled);
    /// Merge with another batch.
    bool Merge(const UIBatch& batch);
    /// Return an interpolated color for the UI element.
    unsigned GetInterpolatedColor(int x, int y);

    /// Add or merge a batch.
    static void AddOrMerge(const UIBatch& batch, PODVector<UIBatch>& batches);

    /// Element this batch represents.
    UIElement* element_;
    /// Blending mode.
    BlendMode blendMode_;
    /// Scissor rectangle.
    IntRect scissor_;
    /// Texture.
    Texture* texture_;
    /// Inverse texture size.
    Vector2 invTextureSize_;
    /// Current color. By default calculated from the element.
    unsigned color_;
    /// Vertex data.
    PODVector<float>* vertexData_;
    /// Vertex data start index.
    unsigned vertexStart_;
    /// Vertex data end index.
    unsigned vertexEnd_;
    /// Gradient flag.
    bool useGradient_;

    /// Position adjustment vector for pixel-perfect rendering. Initialized by UI.
    static Vector3 posAdjust;
};

}
