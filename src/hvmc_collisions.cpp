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
    vec2 taille = box1->collider.dims;
    f32 minAy = box1->position.y - taille.y/2;
    f32 minAx = box1->position.x - taille.x/2;
    f32 maxAx = box1->position.x + taille.x/2;
    f32 maxAy = box1->position.y + taille.y/2;
    f32 minBy = box2->position.y - taille.y/2;
    f32 minBx = box2->position.x - taille.x/2;
    f32 maxBx = box2->position.x + taille.x/2;
    f32 maxBy = box2->position.y + taille.y/2;

    if ((((minAx>minBx)&&(minAx<maxBx))&&((minAy>minBy)&&(minAy<maxBy)))||
            ((((maxAx>minBx)&&(maxAx<maxBx))&&((maxAy>minBy)&&(maxAy<maxBy))))||
            (((minAx>minBx)&&(minAx<maxBx))&&((maxAy>minBy)&&(maxAy<maxBy)))||
            (((maxAx>minBx)&&(maxAx<maxBx))&&((minAy>minBy)&&(minAy<maxBy)))
            ){
        std::cout << "BOX2BOX" << std::endl;
        box1->velocity = {0.0,0.0};
        box2->velocity = {0.0,0.0};
        box1->SetKinematic();box2->SetKinematic();
        /*collision infos*/
        info.rb1 = box1;
        info.rb2 = box2;
        info.p_contact = {f32(sqrt(pow(box1->position.x - box2->position.x,2))/2),f32(sqrt(pow(box1->position.y - box2->position.y,2))/2)};
        info.dp = 42; //on s'en balaicouilles pour l'instant

  /*      if ( && minBy > maxAy )
            info.norm = {1,0};
        else if (  && minAy > maxBy )
            info.norm = {-1,0};
        else if (  && minBx > maxAx )
            info.norm = {0,1};
        else if (  && minAx > maxBx )
            info.norm = {0,-1};
*/

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

        /*collision infos*/
        info.rb1 = circle1;
        info.rb2 = circle2;
        info.dp = rayon2 + rayon1 - sqrt(pow(circle1->position.x - circle2->position.x,2) + pow(circle1->position.y - circle2->position.y,2));
        info.norm = {(circle2->position.x - circle1->position.x) / abs(circle2->position.x - circle1->position.x),
                     (circle2->position.y - circle1->position.y) / abs(circle2->position.y - circle1->position.y)};
        info.p_contact = {circle1->position.x + rayon1 * info.norm.x , circle1->position.y + rayon1 * info.norm.y};
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
    f32 rayon = circle->collider.radius;
    f32 rayon2 = rayon * rayon;
    f32 by = circle->position.y;
    f32 bx = circle->position.x;
    vec2 taille = box->collider.dims;
    f32 minAy = box->position.y - taille.y/2;
    f32 minAx = box->position.x - taille.x/2;
    f32 maxAx = box->position.x + taille.x/2;
    f32 maxAy = box->position.y + taille.y/2;

    f32 py;
    f32 px;
    f32 p;
    f32 pmin=1000000000;

    // Recherche du point p, étant le point le plus près du centre b de la boite
    for (f32 i = minAx; i<= maxAx; i++){
        if (i == minAx || i == maxAx) {
             for (f32 j = minAy; j <= maxAy; j++){

                 p=sqrt( pow((i-bx),2) + pow((j-by),2) );
                 if (p<pmin){
                     pmin=p;
                     px=i;
                     py=j;
                 }
             }
        }
        else {
            for (f32 j = minAy; j <= maxAy;){


                    p=sqrt( pow((i-bx),2) + pow((j-by),2) );
                    if (p<pmin){
                        pmin=p;
                        px=i;
                        py=j;
                    }
                    j+=taille.y;
             }
         }
    }


    f32 norme2 = sqrt( ((px-bx)*(px-bx)) + ((py-by)*(py-by)) );
    norme2 *= norme2;

    if (norme2   < rayon2) {
        //std::cout << "C2B" << std::endl;
        circle->velocity = {0.0,0.0};
        box->velocity = {0.0,0.0};
        circle->SetKinematic();
        box->SetKinematic();

        /*collision infos*/
        info.rb1 = box;
        info.rb2 = circle;
        info.p_contact = {f32(sqrt(pow(box->position.x - circle->position.x,2))/2),f32(sqrt(pow(box->position.y - circle->position.y,2))/2)};
        info.dp = 42; //on s'en balaicouilles pour l'instant

        return true;
    }



    return false;
}
bool collisionWithWall(CollisionInfo & info){
    //std::cout << "WALL-E" << std::endl;

    return false;
}
