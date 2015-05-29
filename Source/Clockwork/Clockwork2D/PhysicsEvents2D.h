#pragma once

#include "../Core/Object.h"

namespace Clockwork
{

/// Physics world is about to be stepped.
EVENT(E_PHYSICSPRESTEP2D, PhysicsPreStep2D)
{
    PARAM(P_WORLD, World);                  // PhysicsWorld2D pointer
    PARAM(P_TIMESTEP, TimeStep);            // float
}

/// Physics world has been stepped.
EVENT(E_PHYSICSPOSTSTEP2D, PhysicsPostStep2D)
{
    PARAM(P_WORLD, World);                  // PhysicsWorld2D pointer
    PARAM(P_TIMESTEP, TimeStep);            // float
}

/// Physics begin contact.
EVENT(E_PHYSICSBEGINCONTACT2D, PhysicsBeginContact2D)
{
    PARAM(P_WORLD, World);                  // PhysicsWorld2D pointer
    PARAM(P_BODYA, BodyA);                  // RigidBody2D pointer
    PARAM(P_BODYB, BodyB);                  // RigidBody2D pointer
    PARAM(P_NODEA, NodeA);                  // Node pointer
    PARAM(P_NODEB, NodeB);                  // Node pointer
}

/// Physics end contact.
EVENT(E_PHYSICSENDCONTACT2D, PhysicsEndContact2D)
{
    PARAM(P_WORLD, World);                  // PhysicsWorld2D pointer
    PARAM(P_BODYA, BodyA);                  // RigidBody2D pointer
    PARAM(P_BODYB, BodyB);                  // RigidBody2D pointer
    PARAM(P_NODEA, NodeA);                  // Node pointer
    PARAM(P_NODEB, NodeB);                  // Node pointer
}

}
