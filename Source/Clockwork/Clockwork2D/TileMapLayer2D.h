

#pragma once

#include "../Scene/Component.h"
#include "../Clockwork2D/TileMapDefs2D.h"

namespace Clockwork
{

class DebugRenderer;
class Node;
class TileMap2D;
class TmxImageLayer2D;
class TmxLayer2D;
class TmxObjectGroup2D;
class TmxTileLayer2D;

/// Tile map component.
class CLOCKWORK_API TileMapLayer2D : public Component
{
    OBJECT(TileMapLayer2D);

public:
    /// Construct.
    TileMapLayer2D(Context* context);
    /// Destruct.
    ~TileMapLayer2D();
    /// Register object factory.
    static void RegisterObject(Context* context);

    /// Add debug geometry to the debug renderer.
    virtual void DrawDebugGeometry(DebugRenderer* debug, bool depthTest);

    /// Initialize with tile map and tmx layer.
    void Initialize(TileMap2D* tileMap, const TmxLayer2D* tmxLayer);
    /// Set draw order
    void SetDrawOrder(int drawOrder);
    /// Set visible.
    void SetVisible(bool visible);

    /// Return tile map.
    TileMap2D* GetTileMap() const;

    /// Return tmx layer.
    const TmxLayer2D* GetTmxLayer() const { return tmxLayer_; }

    /// Return draw order.
    int GetDrawOrder() const { return drawOrder_; }

    /// Return visible.
    bool IsVisible() const { return visible_; }

    /// Return has property
    bool HasProperty(const String& name) const;
    /// Return property.
    const String& GetProperty(const String& name) const;
    /// Return layer type.
    TileMapLayerType2D GetLayerType() const;

    /// Return width (for tile layer only).
    int GetWidth() const;
    /// Return height (for tile layer only).
    int GetHeight() const;
    /// Return tile node (for tile layer only).
    Node* GetTileNode(int x, int y) const;
    /// Return tile (for tile layer only).
    Tile2D* GetTile(int x, int y) const;

    /// Return number of tile map objects (for object group only).
    unsigned GetNumObjects() const;
    /// Return tile map object (for object group only).
    TileMapObject2D* GetObject(unsigned index) const;
    /// Return object node (for object group only).
    Node* GetObjectNode(unsigned index) const;

    /// Return image node (for image layer only).
    Node* GetImageNode() const;

private:
    /// Set tile layer.
    void SetTileLayer(const TmxTileLayer2D* tileLayer);
    /// Set object group.
    void SetObjectGroup(const TmxObjectGroup2D* objectGroup);
    /// Set image layer.
    void SetImageLayer(const TmxImageLayer2D* imageLayer);

    /// Tile map.
    WeakPtr<TileMap2D> tileMap_;
    /// Tmx layer.
    const TmxLayer2D* tmxLayer_;
    /// Tile layer.
    const TmxTileLayer2D* tileLayer_;
    /// Object group.
    const TmxObjectGroup2D* objectGroup_;
    /// Image layer.
    const TmxImageLayer2D* imageLayer_;
    /// Draw order.
    int drawOrder_;
    /// Visible.
    bool visible_;
    /// Tile node or image nodes.
    Vector<SharedPtr<Node> > nodes_;
};

}
