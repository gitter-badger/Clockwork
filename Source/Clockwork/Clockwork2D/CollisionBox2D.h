#pragma once

#include "../Clockwork2D/CollisionShape2D.h"

namespace Clockwork
{

/// 2D box collision component.
class CLOCKWORK_API CollisionBox2D : public CollisionShape2D
{
    OBJECT(CollisionBox2D);

public:
    /// Construct.
    CollisionBox2D(Context* scontext);
    /// Destruct.
    virtual ~CollisionBox2D();
    /// Register object factory.
    static void RegisterObject(Context* context);

    /// Set size.
    void SetSize(const Vector2& size);
    /// Set size.
    void SetSize(float width, float height);
    /// Set center.
    void SetCenter(const Vector2& center);
    /// Set center.
    void SetCenter(float x, float y);
    /// Set angle.
    void SetAngle(float angle);

    /// Return size.
    const Vector2& GetSize() const { return size_; }
    /// Return center.
    const Vector2& GetCenter() const { return center_; }
    /// Return angle.
    float GetAngle() const { return angle_; }

private:
    /// Apply node world scale.
    virtual void ApplyNodeWorldScale();
    /// Recreate fixture.
    void RecreateFixture();

    /// Box shape.
    b2PolygonShape boxShape_;
    /// Size.
    Vector2 size_;
    /// Center
    Vector2 center_;
    /// Angle.
    float angle_;
};

}
