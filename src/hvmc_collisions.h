#ifndef HVMC_COLLISIONS_H
#define HVMC_COLLISIONS_H

#include "hvmc_math.h"
#include "hvmc_physics.h"

#endif

struct CollisionInfo{
    //be my guest
    //add whatever you want
};

struct Collisions{
    //test collision between a and b, gives an info
    bool collider(RigidBody * a, RigidBody * b, CollisionInfo * info);

    //test collision box to box
    bool collisionBox2Box(RigidBody * box1, RigidBody * box2);

    //test collision circle to circle
    bool collisionCircle2Circle(RigidBody * circle1, RigidBody * circle2);

    //test collision box to circle
    bool collisionBox2Circle(RigidBody * box, RigidBody * circle);


    std::vector<CollisionInfo*> collisions; //collisions.push_back(info);
};

