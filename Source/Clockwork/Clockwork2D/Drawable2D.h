#pragma once

#include "../Graphics/Drawable.h"
#include "../Graphics/GraphicsDefs.h"

namespace Clockwork
{

class Renderer2D;
class Texture2D;
class VertexBuffer;

/// 2D vertex.
struct Vertex2D
{
    /// Position.
    Vector3 position_;
    /// Color.
    unsigned color_;
    /// UV.
    Vector2 uv_;
};

/// 2D source batch.
struct SourceBatch2D
{
    /// Construct.
    SourceBatch2D();

    /// Draw order.
    int drawOrder_;
    /// Material.
    SharedPtr<Material> material_;
    /// Vertices.
    Vector<Vertex2D> vertices_;
};

/// Pixel size (equal 0.01f).
extern CLOCKWORK_API const float PIXEL_SIZE;

/// Base class for 2D visible components.
class CLOCKWORK_API Drawable2D : public Drawable
{
    OBJECT(Drawable2D);

public:
    /// Construct.
    Drawable2D(Context* context);
    /// Destruct.
    ~Drawable2D();
    /// Register object factory. Drawable must be registered first.
    static void RegisterObject(Context* context);

    /// Handle enabled/disabled state change.
    virtual void OnSetEnabled();

    /// Set layer.
    void SetLayer(int layer);
    /// Set order in layer.
    void SetOrderInLayer(int orderInLayer);

    /// Return layer.
    int GetLayer() const { return layer_; }
    /// Return order in layer.
    int GetOrderInLayer() const { return orderInLayer_; }

    /// Return all source batches (called by Renderer2D).
    const Vector<SourceBatch2D>& GetSourceBatches();

protected:
    /// Handle node being assigned.
    virtual void OnNodeSet(Node* node);
    /// Handle node transform being dirtied.
    virtual void OnMarkedDirty(Node* node);
    /// Handle draw order changed.
    virtual void OnDrawOrderChanged() = 0;
    /// Update source batches.
    virtual void UpdateSourceBatches() = 0;
    /// Return draw order by layer and order in layer.
    int GetDrawOrder() const { return (layer_ << 20) + (orderInLayer_ << 10); }

    /// Layer.
    int layer_;
    /// Order in layer.
    int orderInLayer_;
    /// Source batches.
    Vector<SourceBatch2D> sourceBatches_;
    /// Source batches dirty flag.
    bool sourceBatchesDirty_;
    /// Renderer2D.
    WeakPtr<Renderer2D> renderer_;
};

}
