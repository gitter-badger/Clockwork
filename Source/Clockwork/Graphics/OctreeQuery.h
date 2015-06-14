#pragma once

#include "../Math/BoundingBox.h"
#include "../Graphics/Drawable.h"
#include "../Math/Frustum.h"
#include "../Math/Ray.h"
#include "../Math/Sphere.h"

namespace Clockwork
{

class Drawable;
class Node;

/// Base class for octree queries.
class CLOCKWORK_API OctreeQuery
{
public:
    /// Construct with query parameters.
    OctreeQuery(PODVector<Drawable*>& result, unsigned char drawableFlags, unsigned viewMask) :
        result_(result),
        drawableFlags_(drawableFlags),
        viewMask_(viewMask)
    {
    }

    /// Destruct.
    virtual ~OctreeQuery()
    {
    }

    /// Intersection test for an octant.
    virtual Intersection TestOctant(const BoundingBox& box, bool inside) = 0;
    /// Intersection test for drawables.
    virtual void TestDrawables(Drawable** start, Drawable** end, bool inside) = 0;

    /// Result vector reference.
    PODVector<Drawable*>& result_;
    /// Drawable flags to include.
    unsigned char drawableFlags_;
    /// Drawable layers to include.
    unsigned viewMask_;

private:
    /// Prevent copy construction.
    OctreeQuery(const OctreeQuery& rhs);
    /// Prevent assignment.
    OctreeQuery& operator = (const OctreeQuery& rhs);
};

/// Point octree query.
class CLOCKWORK_API PointOctreeQuery : public OctreeQuery
{
public:
    /// Construct with point and query parameters.
    PointOctreeQuery(PODVector<Drawable*>& result, const Vector3& point, unsigned char drawableFlags = DRAWABLE_ANY,
        unsigned viewMask = DEFAULT_VIEWMASK) :
        OctreeQuery(result, drawableFlags, viewMask),
        point_(point)
    {
    }

    /// Intersection test for an octant.
    virtual Intersection TestOctant(const BoundingBox& box, bool inside);
    /// Intersection test for drawables.
    virtual void TestDrawables(Drawable** start, Drawable** end, bool inside);

    /// Point.
    Vector3 point_;
};

/// %Sphere octree query.
class CLOCKWORK_API SphereOctreeQuery : public OctreeQuery
{
public:
    /// Construct with sphere and query parameters.
    SphereOctreeQuery(PODVector<Drawable*>& result, const Sphere& sphere, unsigned char drawableFlags = DRAWABLE_ANY,
        unsigned viewMask = DEFAULT_VIEWMASK) :
        OctreeQuery(result, drawableFlags, viewMask),
        sphere_(sphere)
    {
    }

    /// Intersection test for an octant.
    virtual Intersection TestOctant(const BoundingBox& box, bool inside);
    /// Intersection test for drawables.
    virtual void TestDrawables(Drawable** start, Drawable** end, bool inside);

    /// Sphere.
    Sphere sphere_;
};

/// Bounding box octree query.
class CLOCKWORK_API BoxOctreeQuery : public OctreeQuery
{
public:
    /// Construct with bounding box and query parameters.
    BoxOctreeQuery(PODVector<Drawable*>& result, const BoundingBox& box, unsigned char drawableFlags = DRAWABLE_ANY,
        unsigned viewMask = DEFAULT_VIEWMASK) :
        OctreeQuery(result, drawableFlags, viewMask),
        box_(box)
    {
    }

    /// Intersection test for an octant.
    virtual Intersection TestOctant(const BoundingBox& box, bool inside);
    /// Intersection test for drawables.
    virtual void TestDrawables(Drawable** start, Drawable** end, bool inside);

    /// Bounding box.
    BoundingBox box_;
};

/// %Frustum octree query.
class CLOCKWORK_API FrustumOctreeQuery : public OctreeQuery
{
public:
    /// Construct with frustum and query parameters.
    FrustumOctreeQuery(PODVector<Drawable*>& result, const Frustum& frustum, unsigned char drawableFlags = DRAWABLE_ANY,
        unsigned viewMask = DEFAULT_VIEWMASK) :
        OctreeQuery(result, drawableFlags, viewMask),
        frustum_(frustum)
    {
    }

    /// Intersection test for an octant.
    virtual Intersection TestOctant(const BoundingBox& box, bool inside);
    /// Intersection test for drawables.
    virtual void TestDrawables(Drawable** start, Drawable** end, bool inside);

    /// Frustum.
    Frustum frustum_;
};

/// General octree query result. Used for Lua bindings only.
struct CLOCKWORK_API OctreeQueryResult
{
    /// Construct with defaults.
    OctreeQueryResult() :
        drawable_(0),
        node_(0)
    {
    }

    /// Test for inequality, added to prevent GCC from complaining.
    bool operator != (const OctreeQueryResult& rhs) const { return drawable_ != rhs.drawable_ || node_ != rhs.node_; }

    /// Drawable.
    Drawable* drawable_;
    /// Scene node.
    Node* node_;
};

/// Graphics raycast detail level.
enum RayQueryLevel
{
    RAY_AABB = 0,
    RAY_OBB,
    RAY_TRIANGLE
};

/// Raycast result.
struct CLOCKWORK_API RayQueryResult
{
    /// Construct with defaults.
    RayQueryResult() :
        drawable_(0),
        node_(0)
    {
    }

    /// Test for inequality, added to prevent GCC from complaining.
    bool operator != (const RayQueryResult& rhs) const { return position_ != rhs.position_ || normal_ != rhs.normal_ || distance_ != rhs.distance_ || drawable_ != rhs.drawable_ || node_ != rhs.node_ || subObject_ != rhs.subObject_; }

    /// Hit position in world space.
    Vector3 position_;
    /// Hit normal in world space. Negation of ray direction if per-triangle data not available.
    Vector3 normal_;
    /// Distance from ray origin.
    float distance_;
    /// Drawable.
    Drawable* drawable_;
    /// Scene node.
    Node* node_;
    /// Drawable specific subobject if applicable.
    unsigned subObject_;
};

/// Raycast octree query.
class CLOCKWORK_API RayOctreeQuery
{
public:
    /// Construct with ray and query parameters.
    RayOctreeQuery(PODVector<RayQueryResult>& result, const Ray& ray, RayQueryLevel level = RAY_TRIANGLE,
        float maxDistance = M_INFINITY, unsigned char drawableFlags = DRAWABLE_ANY, unsigned viewMask = DEFAULT_VIEWMASK) :
        result_(result),
        ray_(ray),
        drawableFlags_(drawableFlags),
        viewMask_(viewMask),
        maxDistance_(maxDistance),
        level_(level)
    {
    }

    /// Result vector reference.
    PODVector<RayQueryResult>& result_;
    /// Ray.
    Ray ray_;
    /// Drawable flags to include.
    unsigned char drawableFlags_;
    /// Drawable layers to include.
    unsigned viewMask_;
    /// Maximum ray distance.
    float maxDistance_;
    /// Raycast detail level.
    RayQueryLevel level_;

private:
    /// Prevent copy construction.
    RayOctreeQuery(const RayOctreeQuery& rhs);
    /// Prevent assignment.
    RayOctreeQuery& operator = (const RayOctreeQuery& rhs);
};

}