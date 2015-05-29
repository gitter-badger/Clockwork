#pragma once

#include "../Clockwork2D/CollisionShape2D.h"

namespace Clockwork
{
/// 2D chain collision component.
class CLOCKWORK_API CollisionChain2D : public CollisionShape2D
{
    OBJECT(CollisionChain2D);

public:
    /// Construct.
    CollisionChain2D(Context* scontext);
    /// Destruct.
    virtual ~CollisionChain2D();
    /// Register object factory.
    static void RegisterObject(Context* context);

    /// Set loop.
    void SetLoop(bool loop);
    /// Set vertex count.
    void SetVertexCount(unsigned count);
    /// Set vertex.
    void SetVertex(unsigned index, const Vector2& vertex);
    /// Set vertices.
    void SetVertices(const PODVector<Vector2>& vertices);
    /// Set vertices attribute.
    void SetVerticesAttr(const PODVector<unsigned char>& value);

    /// Return loop.
    bool GetLoop() const { return loop_; }
    /// Return vertex count.
    unsigned GetVertexCount() const { return vertices_.Size(); }
    /// Return vertex.
    const Vector2& GetVertex(unsigned index) const { return (index < vertices_.Size()) ? vertices_[index] : Vector2::ZERO; }
    /// Return vertices.
    const PODVector<Vector2>& GetVertices() const { return vertices_; }
    /// Return vertices attribute.
    PODVector<unsigned char> GetVerticesAttr() const;

private:
    /// Apply node world scale.
    virtual void ApplyNodeWorldScale();
    /// Recreate fixture.
    void RecreateFixture();

    /// Chain shape.
    b2ChainShape chainShape_;
    /// Loop.
    bool loop_;
    /// Vertices.
    PODVector<Vector2> vertices_;
};

}
