#pragma once

#include "../Core/Object.h"

namespace Clockwork
{

/// Physics world is about to be stepped.
EVENT(E_PHYSICSPRESTEP, PhysicsPreStep)
{
    PARAM(P_WORLD, World);                  // PhysicsWorld pointer
    PARAM(P_TIMESTEP, TimeStep);            // float
}

/// Physics world has been stepped.
EVENT(E_PHYSICSPOSTSTEP, PhysicsPostStep)
{
    PARAM(P_WORLD, World);                  // PhysicsWorld pointer
    PARAM(P_TIMESTEP, TimeStep);            // float
}

/// Physics collision started.
EVENT(E_PHYSICSCOLLISIONSTART, PhysicsCollisionStart)
{
    PARAM(P_WORLD, World);                  // PhysicsWorld pointer
    PARAM(P_NODEA, NodeA);                  // Node pointer
    PARAM(P_NODEB, NodeB);                  // Node pointer
    PARAM(P_BODYA, BodyA);                  // RigidBody pointer
    PARAM(P_BODYB, BodyB);                  // RigidBody pointer
    PARAM(P_TRIGGER, Trigger);              // bool
    PARAM(P_CONTACTS, Contacts);            // Buffer containing position (Vector3), normal (Vector3), distance (float), impulse (float) for each contact
}

/// Physics collision ongoing.
EVENT(E_PHYSICSCOLLISION, PhysicsCollision)
{
    PARAM(P_WORLD, World);                  // PhysicsWorld pointer
    PARAM(P_NODEA, NodeA);                  // Node pointer
    PARAM(P_NODEB, NodeB);                  // Node pointer
    PARAM(P_BODYA, BodyA);                  // RigidBody pointer
    PARAM(P_BODYB, BodyB);                  // RigidBody pointer
    PARAM(P_TRIGGER, Trigger);              // bool
    PARAM(P_CONTACTS, Contacts);            // Buffer containing position (Vector3), normal (Vector3), distance (float), impulse (float) for each contact
}

/// Physics collision ended.
EVENT(E_PHYSICSCOLLISIONEND, PhysicsCollisionEnd)
{
    PARAM(P_WORLD, World);                  // PhysicsWorld pointer
    PARAM(P_NODEA, NodeA);                  // Node pointer
    PARAM(P_NODEB, NodeB);                  // Node pointer
    PARAM(P_BODYA, BodyA);                  // RigidBody pointer
    PARAM(P_BODYB, BodyB);                  // RigidBody pointer
    PARAM(P_TRIGGER, Trigger);              // bool
}

/// Physics collision started (sent to the participating scene nodes.)
EVENT(E_NODECOLLISIONSTART, NodeCollisionStart)
{
    PARAM(P_BODY, Body);                    // RigidBody pointer
    PARAM(P_OTHERNODE, OtherNode);          // Node pointer
    PARAM(P_OTHERBODY, OtherBody);          // RigidBody pointer
    PARAM(P_TRIGGER, Trigger);              // bool
    PARAM(P_CONTACTS, Contacts);            // Buffer containing position (Vector3), normal (Vector3), distance (float), impulse (float) for each contact
}

/// Physics collision ongoing (sent to the participating scene nodes.)
EVENT(E_NODECOLLISION, NodeCollision)
{
    PARAM(P_BODY, Body);                    // RigidBody pointer
    PARAM(P_OTHERNODE, OtherNode);          // Node pointer
    PARAM(P_OTHERBODY, OtherBody);          // RigidBody pointer
    PARAM(P_TRIGGER, Trigger);              // bool
    PARAM(P_CONTACTS, Contacts);            // Buffer containing position (Vector3), normal (Vector3), distance (float), impulse (float) for each contact
}

/// Physics collision ended (sent to the participating scene nodes.)
EVENT(E_NODECOLLISIONEND, NodeCollisionEnd)
{
    PARAM(P_BODY, Body);                    // RigidBody pointer
    PARAM(P_OTHERNODE, OtherNode);          // Node pointer
    PARAM(P_OTHERBODY, OtherBody);          // RigidBody pointer
    PARAM(P_TRIGGER, Trigger);              // bool
}

}
