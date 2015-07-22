

#pragma once

#include "../Clockwork2D/CollisionShape2D.h"

namespace Clockwork
{

/// 2D circle collision component.
class CLOCKWORK_API CollisionCircle2D : public CollisionShape2D
{
    OBJECT(CollisionCircle2D);

public:
    /// Construct.
    CollisionCircle2D(Context* context);
    /// Destruct.
    virtual ~CollisionCircle2D();
    /// Register object factory.
    static void RegisterObject(Context* context);

    /// Set radius.
    void SetRadius(float radius);
    /// Set center.
    void SetCenter(const Vector2& center);
    /// Set center.
    void SetCenter(float x, float y);

    /// Return radius.
    float GetRadius() const { return radius_; }

    /// Return center.
    const Vector2& GetCenter() const { return center_; }

private:
    /// Apply node world scale.
    virtual void ApplyNodeWorldScale();
    /// Recreate fixture.
    void RecreateFixture();

    /// Circle shape.
    b2CircleShape circleShape_;
    /// Radius.
    float radius_;
    /// Center.
    Vector2 center_;
};

}
