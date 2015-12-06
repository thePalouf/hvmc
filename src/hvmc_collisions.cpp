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
    f32 posy2 = box2->position.y ;
    f32 posy1 = box1->position.y;
    f32 posx2 = box2->position.x;
    f32 posx1 = box1->position.x;
    f32 penetrationX=abs(posx1-posx2);
    f32 penetrationY=abs(posy1-posy2);

    if ((((minAx>minBx)&&(minAx<maxBx))&&((minAy>minBy)&&(minAy<maxBy)))||
            ((((maxAx>minBx)&&(maxAx<maxBx))&&((maxAy>minBy)&&(maxAy<maxBy))))||
            (((minAx>minBx)&&(minAx<maxBx))&&((maxAy>minBy)&&(maxAy<maxBy)))||
            (((maxAx>minBx)&&(maxAx<maxBx))&&((minAy>minBy)&&(minAy<maxBy)))
            ){
        std::cout << "BOX2BOX" << std::endl;
        /*box1->velocity = {0.0,0.0};
        box2->velocity = {0.0,0.0};
        box1->SetKinematic();box2->SetKinematic();*/
        /*collision infos*/
        info.rb1 = box1;
        info.rb2 = box2;
        //info.p_contact = {f32(sqrt(pow(box1->position.x - box2->position.x,2))/2),f32(sqrt(pow(box1->position.y - box2->position.y,2))/2)};
        (penetrationX<penetrationY) ? info.dp = penetrationX : info.dp = penetrationY;

        if ((penetrationX<penetrationY) && posy2 > posy1 ){
            info.norm = {1,0};
            info.p_contact.y=posy1+((posy2-posy1)/2);
            (posx1>posx2) ? info.p_contact.x=posx2+((posx1-posx2)/2) :info.p_contact.x=posx1+((posx2-posx1)/2) ;
        }
        else if ( (penetrationX<penetrationY) && posy1 > posy2 ){
            info.norm = {-1,0};
            info.p_contact.y=posy2+((posy1-posy2)/2);
            (posx1>posx2) ? info.p_contact.x=posx2+((posx1-posx2)/2) :info.p_contact.x=posx1+((posx2-posx1)/2) ;
        }
        else if ((penetrationY<penetrationX)  && posx2 > posx1 ){
            info.norm = {0,1};
            info.p_contact.x=posx1+((posx2-posx1)/2);
            (posy1>posy2) ? info.p_contact.y=posy2+((posy1-posy2)/2) :info.p_contact.y=posy1+((posy2-posy1)/2) ;
        }
        else if ( (penetrationY<penetrationX) && posx1 > posx2 ){
            info.norm = {0,-1};
            info.p_contact.x=posx2+((posx1-posx2)/2);
            (posy1>posy2) ? info.p_contact.y=posy2+((posy1-posy2)/2) :info.p_contact.y=posy1+((posy2-posy1)/2) ;
        }

//        std::cout << "Ça fonctionne : "<<penetrationX<<" "<<penetrationY<<" "<<posx1<<" "<<posy1<<" "<<posx2<<" "<<posy2<<" ("<<info.p_contact.x<<","<<info.p_contact.y<<")"<< std::endl;

        return true;
    }
    else
    {
        return false;
    }
    //return false;
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
        std::cout << "C2C" << std::endl;
        /*circle1->velocity = {0.0,0.0};
        circle2->velocity = {0.0,0.0};
        circle1->SetKinematic();circle2->SetKinematic();*/

        /*collision infos*/
        info.rb1 = circle1;
        info.rb2 = circle2;
        info.dp = rayon2 + rayon1 - sqrt(pow(circle1->position.x - circle2->position.x,2) + pow(circle1->position.y - circle2->position.y,2));
        info.norm = (circle2->position - circle1->position) / sqrt(pow(circle1->position.x - circle2->position.x,2) + pow(circle1->position.y - circle2->position.y,2));

        info.p_contact = circle1->position + rayon1*info.norm;

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
        std::cout << "C2B" << std::endl;
        /*circle->velocity = {0.0,0.0};
        box->velocity = {0.0,0.0};
        circle->SetKinematic();
        box->SetKinematic();*/

        /*collision infos*/
//        info.rb1 = box;
//        info.rb2 = circle;
//        info.p_contact = {f32(sqrt(pow(box->position.x - circle->position.x,2))/2),f32(sqrt(pow(box->position.y - circle->position.y,2))/2)};
//        info.dp = 42; //on s'en balaicouilles pour l'instant


        /*collision infos*/
        info.rb1 = box;
        info.rb2 = circle;
        //info.p_contact = {f32(sqrt(pow(box->position.x - circle->position.x,2))/2),f32(sqrt(pow(box->position.y - circle->position.y,2))/2)};
        info.p_contact = {px,py};
        info.dp = abs(rayon - sqrt(norme2)); // rayon du cercle moins la distance entre le point(du carré) le plus proche du centre du cercle et le centre du cercle
//        Pas bon ça :
        info.norm = (circle->position - box->position) / sqrt(pow(box->position.x - circle->position.x,2) + pow(box->position.y - circle->position.y,2));



        return true;
    }



    return false;
}
bool collisionWithWall(RigidBody * rb1, RigidBody * rb2,CollisionInfo & info){
    //std::cout << "WALL-E" << std::endl;

    return false;
}
