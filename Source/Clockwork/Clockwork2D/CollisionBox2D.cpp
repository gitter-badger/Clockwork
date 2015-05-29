#include "../Core/Context.h"
#include "../Clockwork2D/CollisionBox2D.h"
#include "../Clockwork2D/PhysicsUtils2D.h"

#include "../DebugNew.h"

namespace Clockwork
{

extern const char* CLOCKWORK2D_CATEGORY;
static const Vector2 DEFAULT_BOX_SIZE(0.01f, 0.01f);

CollisionBox2D::CollisionBox2D(Context* context) :
    CollisionShape2D(context),
    size_(DEFAULT_BOX_SIZE),
    center_(Vector2::ZERO),
    angle_(0.0f)
{
    float halfWidth = size_.x_ * 0.5f * cachedWorldScale_.x_;
    float halfHeight = size_.y_ * 0.5f * cachedWorldScale_.y_;
    boxShape_.SetAsBox(halfWidth, halfHeight);
    fixtureDef_.shape = &boxShape_;
}

CollisionBox2D::~CollisionBox2D()
{
}

void CollisionBox2D::RegisterObject(Context* context)
{
    context->RegisterFactory<CollisionBox2D>(CLOCKWORK2D_CATEGORY);

    ACCESSOR_ATTRIBUTE("Is Enabled", IsEnabled, SetEnabled, bool, true, AM_DEFAULT);
    ACCESSOR_ATTRIBUTE("Size", GetSize, SetSize, Vector2, DEFAULT_BOX_SIZE, AM_DEFAULT);
    ACCESSOR_ATTRIBUTE("Center", GetCenter, SetCenter, Vector2, Vector2::ZERO, AM_DEFAULT);
    ACCESSOR_ATTRIBUTE("Angle", GetAngle, SetAngle, float, 0.0f, AM_DEFAULT);
    COPY_BASE_ATTRIBUTES(CollisionShape2D);
}

void CollisionBox2D::SetSize(const Vector2& size)
{
    if (size == size_)
        return;

    size_ = size;

    MarkNetworkUpdate();
    RecreateFixture();
}

void CollisionBox2D::SetSize(float width, float height)
{
    SetSize(Vector2(width, height));
}

void CollisionBox2D::SetCenter(const Vector2& center)
{
    if (center == center_)
        return;

    center_ = center;

    MarkNetworkUpdate();
    RecreateFixture();
}

void CollisionBox2D::SetCenter(float x, float y)
{
    SetCenter(Vector2(x, y));
}

void CollisionBox2D::SetAngle(float angle)
{
    if (angle == angle_)
        return;

    angle_ = angle;

    MarkNetworkUpdate();
    RecreateFixture();
}

void CollisionBox2D::ApplyNodeWorldScale()
{
    RecreateFixture();
}

void CollisionBox2D::RecreateFixture()
{
    ReleaseFixture();

    float worlsScaleX = cachedWorldScale_.x_;
    float worldScaleY = cachedWorldScale_.y_;
    float halfWidth = size_.x_ * 0.5f * worlsScaleX;
    float halfHeight = size_.y_ * 0.5f * worldScaleY;
    Vector2 scaledCenter = center_ * Vector2(worlsScaleX, worldScaleY);

    if (scaledCenter == Vector2::ZERO && angle_ == 0.0f)
        boxShape_.SetAsBox(halfWidth, halfHeight);
    else
        boxShape_.SetAsBox(halfWidth, halfHeight, ToB2Vec2(scaledCenter), angle_ * M_DEGTORAD);

    CreateFixture();
}

}
