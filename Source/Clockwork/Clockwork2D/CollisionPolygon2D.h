

#pragma once

#include "../Clockwork2D/CollisionShape2D.h"

namespace Clockwork
{

/// 2D polygon collision component.
class CLOCKWORK_API CollisionPolygon2D : public CollisionShape2D
{
    OBJECT(CollisionPolygon2D);

public:
    /// Construct.
    CollisionPolygon2D(Context* context);
    /// Destruct.
    virtual ~CollisionPolygon2D();
    /// Register object factory.
    static void RegisterObject(Context* context);

    /// Set vertex count.
    void SetVertexCount(unsigned count);
    /// Set vertex.
    void SetVertex(unsigned index, const Vector2& vertex);
    /// Set vertices.
    void SetVertices(const PODVector<Vector2>& vertices);

    /// Return vertex count.
    unsigned GetVertexCount() const { return vertices_.Size(); }

    /// Return vertex.
    const Vector2& GetVertex(unsigned index) const { return (index < vertices_.Size()) ? vertices_[index] : Vector2::ZERO; }

    /// Return vertices.
    const PODVector<Vector2>& GetVertices() const { return vertices_; }

    /// Set vertices attribute.
    void SetVerticesAttr(const PODVector<unsigned char>& value);
    /// Return vertices attribute.
    PODVector<unsigned char> GetVerticesAttr() const;

private:
    /// Apply node world scale.
    virtual void ApplyNodeWorldScale();
    /// Recreate fixture.
    void RecreateFixture();

    /// Polygon shape.
    b2PolygonShape polygonShape_;
    /// Vertices.
    PODVector<Vector2> vertices_;
};

}
