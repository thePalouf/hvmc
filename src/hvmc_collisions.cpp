#include "hvmc_collisions.h"

//test collision between a and b, gives an info
bool Collisions::collider(RigidBody * a, RigidBody * b, CollisionInfo * info){
    //circle2circle
    if ((a->collider.type == RIGID_BODY_SPHERE) && (b->collider.type == RIGID_BODY_SPHERE)){
        return collisionCircle2Circle(a,b,info);
    }
    //box2box
    if ((a->collider.type == RIGID_BODY_BOX) && (b->collider.type == RIGID_BODY_BOX)){
        return collisionBox2Box(a,b,info);
    }

    //box2circle
    if ((a->collider.type == RIGID_BODY_SPHERE) && (b->collider.type == RIGID_BODY_BOX)){
        return collisionBox2Circle(b,a,info);
    }
    else if ((a->collider.type == RIGID_BODY_BOX) && (b->collider.type == RIGID_BODY_SPHERE)){
        return collisionBox2Circle(a,b,info);
    }
}

//test collision box to box
bool Collisions::collisionBox2Box(RigidBody * box1, RigidBody * box2, CollisionInfo * info){
    return false;
}

//test collision circle to circle
bool Collisions::collisionCircle2Circle(RigidBody * circle1, RigidBody * circle2, CollisionInfo * info){
    return false;
}

//test collision box to circle
bool Collisions::collisionBox2Circle(RigidBody * box, RigidBody * circle, CollisionInfo * info){
    return false;
}
