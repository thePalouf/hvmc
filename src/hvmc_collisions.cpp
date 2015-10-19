#include "hvmc_collisions.h"
#include "iostream"

//test collision between a and b, gives an info
bool collider(RigidBody * a, RigidBody * b, CollisionInfo * info){
    if ((a->collider.type == RIGID_BODY_SPHERE) && (b->collider.type == RIGID_BODY_SPHERE)){
        return collisionCircle2Circle(a,b,info);
    }
    else
    if ((a->collider.type == RIGID_BODY_BOX) && (b->collider.type == RIGID_BODY_BOX)){
        return collisionBox2Box(a,b,info);
    }
    else if ((a->collider.type == RIGID_BODY_SPHERE) && (b->collider.type == RIGID_BODY_BOX)){
        return collisionBox2Circle(b,a,info);
    }
    else if ((a->collider.type == RIGID_BODY_BOX) && (b->collider.type == RIGID_BODY_SPHERE)){
        return collisionBox2Circle(a,b,info);
    }
    else //suppose to be contact with wall here (a whatsoever rigid \wall, b wall)
        return collisionWithWall(info); //not really important but who knows ?
}

//test collision box to box
bool collisionBox2Box(RigidBody * box1, RigidBody * box2, CollisionInfo * info){
    std::cout << "BOX2BOX" << std::endl;
    return false;
}

//test collision circle to circle
bool collisionCircle2Circle(RigidBody * circle1, RigidBody * circle2, CollisionInfo * info){
    std::cout << "C2C" << std::endl;
    return false;
}

//test collision box to circle
bool collisionBox2Circle(RigidBody * box, RigidBody * circle, CollisionInfo * info){
    std::cout << "BOX2CIRCLE" << std::endl;
    return false;
}

bool collisionWithWall(CollisionInfo * info){
    std::cout << "WALL-E" << std::endl;
    return false;
}
