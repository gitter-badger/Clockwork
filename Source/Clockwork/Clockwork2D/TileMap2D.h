#pragma once

#include "../Scene/Component.h"
#include "../Clockwork2D/TileMapDefs2D.h"

namespace Clockwork
{

class TileMapLayer2D;
class TmxFile2D;

/// Tile map component.
class CLOCKWORK_API TileMap2D : public Component
{
    OBJECT(TileMap2D);

public:
    /// Construct.
    TileMap2D(Context* context);
    /// Destruct.
    ~TileMap2D();
    /// Register object factory.
    static void RegisterObject(Context* context);

    /// Visualize the component as debug geometry.
    virtual void DrawDebugGeometry(DebugRenderer* debug, bool depthTest);

    /// Set tmx file.
    void SetTmxFile(TmxFile2D* tmxFile);
    /// Add debug geometry to the debug renderer.
    void DrawDebugGeometry();

    /// Return tmx file.
    TmxFile2D* GetTmxFile() const;
    /// Return information.
    const TileMapInfo2D& GetInfo() const { return info_; }
    /// Return number of layers.
    unsigned GetNumLayers() const { return layers_.Size(); }
    /// Return tile map layer at index.
    TileMapLayer2D* GetLayer(unsigned index) const;
    /// Convert tile index to position.
    Vector2 TileIndexToPosition(int x, int y) const;
    /// Convert position to tile index, if out of map return false.
    bool PositionToTileIndex(int& x, int& y, const Vector2& position) const;

    /// Set tile map file attribute.
    void SetTmxFileAttr(const ResourceRef& value);
    /// Return tile map file attribute.
    ResourceRef GetTmxFileAttr() const;

private:
    /// Handle node being assigned.
    virtual void OnNodeSet(Node* node);

    /// Tmx file.
    SharedPtr<TmxFile2D> tmxFile_;
    /// Tile map information.
    TileMapInfo2D info_;
    /// Root node for tile map layer.
    SharedPtr<Node> rootNode_;
    /// Tile map layers.
    Vector<WeakPtr<TileMapLayer2D> > layers_;
};

}
