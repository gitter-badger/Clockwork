#pragma once

#include "../Navigation/NavigationMesh.h"

class dtTileCache;
struct dtTileCacheAlloc;
struct dtTileCacheCompressor;
struct dtTileCacheMeshProcess;
struct dtTileCacheLayer;
struct dtTileCacheContourSet;
struct dtTileCachePolyMesh;

namespace Clockwork
{

class OffMeshConnection;
class Obstacle;

class CLOCKWORK_API DynamicNavigationMesh : public NavigationMesh
{
    OBJECT(DynamicNavigationMesh)
    friend class Obstacle;
    friend struct MeshProcess;

public:
    /// Constructor.
    DynamicNavigationMesh(Context*);
    /// Destructor.
    virtual ~DynamicNavigationMesh();

    /// Register with engine context.
    static void RegisterObject(Context*);

    /// Build/rebuild the entire navigation mesh.
    virtual bool Build();
    /// Build/rebuild a portion of the navigation mesh.
    virtual bool Build(const BoundingBox& boundingBox);
    /// Visualize the component as debug geometry.
    virtual void DrawDebugGeometry(DebugRenderer* debug, bool depthTest);
    /// Add debug geometry to the debug renderer.
    void DrawDebugGeometry(bool depthTest);

    /// Set navigation data attribute.
    virtual void SetNavigationDataAttr(const PODVector<unsigned char>& value);
    /// Return navigation data attribute.
    virtual PODVector<unsigned char> GetNavigationDataAttr() const;

    /// Set the maximum number of obstacles allowed.
    void SetMaxObstacles(unsigned maxObstacles) { maxObstacles_ = maxObstacles; }
    /// Return the maximum number of obstacles allowed.
    unsigned GetMaxObstacles() const { return maxObstacles_; }

    /// Draw debug geometry for Obstacles.
    void SetDrawObstacles(bool enable) { drawObstacles_ = enable; }
    /// Return whether to draw Obstacles.
    bool GetDrawObstacles() const { return drawObstacles_; }

protected:
    struct TileCacheData;

    /// Subscribe to events when assigned to a node.
    virtual void OnNodeSet(Node*);
    /// Trigger the tile cache to make updates to the nav mesh if necessary.
    void HandleSceneSubsystemUpdate(StringHash eventType, VariantMap& eventData);

    /// Used by Obstacle class to add itself to the tile cache, if 'silent' an event will not be raised.
    void AddObstacle(Obstacle* obstacle, bool silent = false);
    /// Used by Obstacle class to update itself.
    void ObstacleChanged(Obstacle* obstacle);
    /// Used by Obstacle class to remove itself from the tile cache, if 'silent' an event will not be raised.
    void RemoveObstacle(Obstacle*, bool silent = false);

    /// Build one tile of the navigation mesh. Return true if successful.
    int BuildTile(Vector<NavigationGeometryInfo>& geometryList, int x, int z, TileCacheData*);
    /// Off-mesh connections to be rebuilt in the mesh processor.
    PODVector<OffMeshConnection*> CollectOffMeshConnections(const BoundingBox& bounds);
    /// Release the navigation mesh, query, and tile cache.
    virtual void ReleaseNavigationMesh();

private:
    /// Free the tile cache.
    void ReleaseTileCache();

    /// Detour tile cache instance that works with the nav mesh.
    dtTileCache* tileCache_;
    /// Used by dtTileCache to allocate blocks of memory.
    dtTileCacheAlloc* allocator_;
    /// Used by dtTileCache to compress the original tiles to use when reconstructing for changes.
    dtTileCacheCompressor* compressor_;
    /// Mesh processer used by Detour, in this case a 'pass-through' processor.
    dtTileCacheMeshProcess* meshProcessor_;
    /// Maximum number of obstacle objects allowed.
    unsigned maxObstacles_;
    /// Debug draw Obstacles.
    bool drawObstacles_;
};

}
