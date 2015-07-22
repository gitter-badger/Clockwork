

#pragma once

#include "../Clockwork2D/CollisionShape2D.h"

namespace Clockwork
{

/// 2D edge collision component.
class CLOCKWORK_API CollisionEdge2D : public CollisionShape2D
{
    OBJECT(CollisionEdge2D);

public:
    /// Construct.
    CollisionEdge2D(Context* context);
    /// Destruct.
    virtual ~CollisionEdge2D();
    /// Register object factory.
    static void RegisterObject(Context* context);

    /// Set vertex 1.
    void SetVertex1(const Vector2& vertex);
    /// Set vertex 2.
    void SetVertex2(const Vector2& vertex);
    /// Set vertices.
    void SetVertices(const Vector2& vertex1, const Vector2& vertex2);

    /// Return vertex 1.
    const Vector2& GetVertex1() const { return vertex1_; }

    /// Return vertex 2.
    const Vector2& GetVertex2() const { return vertex2_; }

private:
    /// Apply node world scale.
    virtual void ApplyNodeWorldScale();
    /// Recreate fixture.
    void RecreateFixture();

    /// Edge shape.
    b2EdgeShape edgeShape_;
    /// Vertex 1.
    Vector2 vertex1_;
    /// Vertex 2.
    Vector2 vertex2_;
};

}
