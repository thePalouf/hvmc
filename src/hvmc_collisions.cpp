#include "hvmc_collisions.h"
#include "iostream"
//test collision between a and b, gives an info
bool collider(RigidBody * a, RigidBody * b, CollisionInfo & info){
    if (a->collider.type==RIGID_BODY_BOX)
    {
        if (b->collider.type==RIGID_BODY_BOX)
            return collisionBox2Box(a,b,info);
        else if(b->collider.type==RIGID_BODY_SPHERE)
        {
            return collisionBox2Circle(a,b,info);
        }
    }
    else if(a->collider.type== RIGID_BODY_SPHERE)
    {
        if(b->collider.type==RIGID_BODY_SPHERE){
            return collisionCircle2Circle(a,b,info);
        }
        else if (b->collider.type==RIGID_BODY_BOX)
            return collisionBox2Circle(b,a,info);
    }
    return false;
}

//test collision box to box
bool collisionBox2Box(RigidBody * box1, RigidBody * box2, CollisionInfo & info){
    //vec2 taille = box1->collider.dims;
    //f32 minAy = box1->position.y - taille.y/2;
    //f32 minAx = box1->position.x - taille.x/2;
    //f32 maxAx = box1->position.x + taille.x/2;
    //f32 maxAy = box1->position.y + taille.y/2;
    //f32 minBy = box2->position.y - taille.y/2;
    //f32 minBx = box2->position.x - taille.x/2;
    //f32 maxBx = box2->position.x + taille.x/2;
    //f32 maxBy = box2->position.y + taille.y/2;

    if (false){
        std::cout << "BOX2BOX" << std::endl;
        box1->velocity = {0.0,0.0};
        box2->velocity = {0.0,0.0};
        box1->SetKinematic();box2->SetKinematic();
        return true;
    }
    else
    {
        return false;
    }
    return false;
}
//test collision circle to circle
bool collisionCircle2Circle(RigidBody * circle1, RigidBody * circle2, CollisionInfo & info){
    //std::cout << "C2C" << std::endl;
    f32 rayon1 = circle1->collider.radius;
    f32 rayon2 = circle2->collider.radius;
    f32 distx=pow(circle1->position.x-circle2->position.x,2);
    f32 disty=pow(circle1->position.y-circle2->position.y,2);
    f32 dist=sqrt(distx+disty);
    if ((rayon1+rayon2)>dist){
        //std::cout << "C2C" << std::endl;
        circle1->velocity = {0.0,0.0};
        circle2->velocity = {0.0,0.0};
        circle1->SetKinematic();circle2->SetKinematic();
        return true;
    }
    else
    {
        //std::cout << "pas bon C2C" << std::endl;
        return false;
    }
}
//test collision box to circle
bool collisionBox2Circle(RigidBody * box, RigidBody * circle, CollisionInfo & info){
    //std::cout << "BOX2CIRCLE" << std::endl;
    return false;
}
bool collisionWithWall(CollisionInfo & info){
    //std::cout << "WALL-E" << std::endl;

    return false;
}
