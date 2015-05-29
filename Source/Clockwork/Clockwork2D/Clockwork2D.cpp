
#include "../Clockwork2D/AnimatedSprite2D.h"
#include "../Clockwork2D/AnimationSet2D.h"
#include "../Clockwork2D/CollisionBox2D.h"
#include "../Clockwork2D/CollisionChain2D.h"
#include "../Clockwork2D/CollisionCircle2D.h"
#include "../Clockwork2D/CollisionEdge2D.h"
#include "../Clockwork2D/CollisionPolygon2D.h"
#include "../Clockwork2D/CollisionShape2D.h"
#include "../Clockwork2D/Constraint2D.h"
#include "../Clockwork2D/ConstraintDistance2D.h"
#include "../Clockwork2D/ConstraintFriction2D.h"
#include "../Clockwork2D/ConstraintGear2D.h"
#include "../Clockwork2D/ConstraintMotor2D.h"
#include "../Clockwork2D/ConstraintMouse2D.h"
#include "../Clockwork2D/ConstraintPrismatic2D.h"
#include "../Clockwork2D/ConstraintPulley2D.h"
#include "../Clockwork2D/ConstraintRevolute2D.h"
#include "../Clockwork2D/ConstraintRope2D.h"
#include "../Clockwork2D/ConstraintWeld2D.h"
#include "../Clockwork2D/ConstraintWheel2D.h"
#include "../Core/Context.h"
#include "../Clockwork2D/Drawable2D.h"
#include "../Clockwork2D/ParticleEffect2D.h"
#include "../Clockwork2D/ParticleEmitter2D.h"
#include "../Clockwork2D/PhysicsWorld2D.h"
#include "../Clockwork2D/Renderer2D.h"
#include "../Clockwork2D/RigidBody2D.h"
#include "../Clockwork2D/Sprite2D.h"
#include "../Clockwork2D/SpriteSheet2D.h"
#include "../Clockwork2D/StaticSprite2D.h"
#include "../Clockwork2D/TileMap2D.h"
#include "../Clockwork2D/TileMapLayer2D.h"
#include "../Clockwork2D/TmxFile2D.h"

#include "../DebugNew.h"

namespace Clockwork
{

const char* CLOCKWORK2D_CATEGORY = "Clockwork2D";

void RegisterClockwork2DLibrary(Context* context)
{
    Renderer2D::RegisterObject(context);

    Sprite2D::RegisterObject(context);
    SpriteSheet2D::RegisterObject(context);

    // Must register objects from base to derived order
    Drawable2D::RegisterObject(context);
    StaticSprite2D::RegisterObject(context);

    AnimationSet2D::RegisterObject(context);
    AnimatedSprite2D::RegisterObject(context);

    ParticleEffect2D::RegisterObject(context);
    ParticleEmitter2D::RegisterObject(context);

    TmxFile2D::RegisterObject(context);
    TileMap2D::RegisterObject(context);
    TileMapLayer2D::RegisterObject(context);

    PhysicsWorld2D::RegisterObject(context);
    RigidBody2D::RegisterObject(context);

    CollisionShape2D::RegisterObject(context);
    CollisionBox2D::RegisterObject(context);
    CollisionChain2D::RegisterObject(context);
    CollisionCircle2D::RegisterObject(context);
    CollisionEdge2D::RegisterObject(context);
    CollisionPolygon2D::RegisterObject(context);

    Constraint2D::RegisterObject(context);
    ConstraintDistance2D::RegisterObject(context);
    ConstraintFriction2D::RegisterObject(context);
    ConstraintGear2D::RegisterObject(context);
    ConstraintMotor2D::RegisterObject(context);
    ConstraintMouse2D::RegisterObject(context);
    ConstraintPrismatic2D::RegisterObject(context);
    ConstraintPulley2D::RegisterObject(context);
    ConstraintRevolute2D::RegisterObject(context);
    ConstraintRope2D::RegisterObject(context);
    ConstraintWeld2D::RegisterObject(context);
    ConstraintWheel2D::RegisterObject(context);
}

}
