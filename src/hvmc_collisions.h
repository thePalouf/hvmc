#ifndef HVMC_COLLISIONS_H
#define HVMC_COLLISIONS_H

#include "hvmc_math.h"
#include "hvmc_physics.h"

#endif
struct RigidBody;
struct CollisionInfo;

//test collision between a and b, gives an info
bool collider(RigidBody * a, RigidBody * b, CollisionInfo & info);

//test collision box to box
bool collisionBox2Box(RigidBody * box1, RigidBody * box2, CollisionInfo & info);

//test collision circle to circle
bool collisionCircle2Circle(RigidBody * circle1, RigidBody * circle2, CollisionInfo & info);

//test collision box to circle
bool collisionBox2Circle(RigidBody * box, RigidBody * circle, CollisionInfo & info);


