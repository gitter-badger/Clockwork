#pragma once

#include "../Math/Color.h"
#include "../Math/Vector2.h"
#include "../Math/Vector3.h"

#include <Box2D/Box2D.h>

namespace Clockwork
{

inline Color ToColor(const b2Color& color)
{
    return Color(color.r, color.g, color.b);
}

inline b2Vec2 ToB2Vec2(const Vector2& vector)
{
    return b2Vec2(vector.x_, vector.y_);
}

inline Vector2 ToVector2(const b2Vec2& vec2)
{
    return Vector2(vec2.x, vec2.y);
}

inline b2Vec2 ToB2Vec2(const Vector3& vector)
{
    return b2Vec2(vector.x_, vector.y_);
}

inline Vector3 ToVector3(const b2Vec2& vec2)
{
    return Vector3(vec2.x, vec2.y, 0.0f);
}

/*
inline bool operator == (const b2Vec2& left, const b2Vec2& right)
{
    return left.x == right.x && left.y == right.y;
}
*/

inline bool operator != (const b2Vec2& left, const b2Vec2& right)
{
    return left.x != right.x || left.y != right.y;
}

}
