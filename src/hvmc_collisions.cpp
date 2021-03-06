#include "hvmc_collisions.h"
#include "iostream"
#include "limits"

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
    f32 posy2 = box2->position.y;
    f32 posy1 = box1->position.y;
    f32 posx2 = box2->position.x;
    f32 posx1 = box1->position.x;
    f32 penetrationX;
    f32 penetrationY;

    if ((((minAx>minBx)&&(minAx<maxBx))&&((minAy>minBy)&&(minAy<maxBy)))||
            ((((maxAx>minBx)&&(maxAx<maxBx))&&((maxAy>minBy)&&(maxAy<maxBy))))||
            (((minAx>minBx)&&(minAx<maxBx))&&((maxAy>minBy)&&(maxAy<maxBy)))||
            (((maxAx>minBx)&&(maxAx<maxBx))&&((minAy>minBy)&&(minAy<maxBy)))
            ){
        //jusqu'ici on est d'accord, après ce gros if des enfers, on a une collision de détectée

        /*collision infos*/
        info.rb1 = box1;
        info.rb2 = box2;

        //calcul penetration en X et Y
        if ((posx1 > posx2))
            penetrationX = (maxBx - minAx);
        else
            penetrationX = (maxAx - minBx);

        if ((posy1 > posy2))
            penetrationY = (maxBy - minAy);
        else
            penetrationY = (maxAy - minBy);

        (penetrationX<penetrationY) ? info.dp = penetrationX : info.dp = penetrationY;

        //ajout info collision sur normales + point de contact
        if ((penetrationX<penetrationY) && posx2 > posx1 ){
            info.norm = {1,0};
            //differenciation entre box et wall
            if(box1->collider.dims.y > 10.f){
                info.p_contact = {posx2, posy2};
            }
            //differenciation entre box et wall
            else if(box2->collider.dims.y>10.f){
                info.p_contact = {posx1, posy1};
            }
            else {//cas de base box to box
                info.p_contact.x=posx1+(posx2-posx1)/2;
                (posy1>posy2) ? info.p_contact.y=posy2+((posy1-posy2)/2) :info.p_contact.y=posy1+((posy2-posy1)/2);
            }
        }
        else if ( (penetrationX<penetrationY) && posx1 > posx2 ){
            info.norm = {-1,0};
            //differenciation entre box et wall
            if(box1->collider.dims.y>10.f) {
                info.p_contact = {posx2, posy2};
            }
            //differenciation entre box et wall
            else if(box2->collider.dims.y>10.f){
                info.p_contact = {posx1, posy1};
            }
            else {//cas de base box to box
                info.p_contact.x=posx2+((posx1-posx2)/2);
                (posy1>posy2) ? info.p_contact.y=posy2+((posy1-posy2)/2) :info.p_contact.y=posy1+((posy2-posy1)/2) ;
            }
        }
        else if ((penetrationY<penetrationX)  && posy2 > posy1 ){
            info.norm = {0,1};
            //differenciation entre box et wall
            if(box1->collider.dims.x>10.f)  {
                info.p_contact = {posx2, posy2};
            }
            //differenciation entre box et wall
            else if(box2->collider.dims.x>10.f){
                info.p_contact = {posx1, posy1};
            }
            else {//cas de base box to box
                info.p_contact.y=posy1+((posy2-posy1)/2);
                (posx1>posx2) ? info.p_contact.x=posx2+((posx1-posx2)/2) :info.p_contact.y=posx1+((posx2-posx1)/2) ;
            }
        }
        else if ( (penetrationY<penetrationX) && posy1 > posy2 ){
            info.norm = {0,-1};
            //differenciation entre box et wall
            if(box1->collider.dims.x>10.f)  {
                info.p_contact = {posx2, posy2};
            }
            //differenciation entre box et wall
            else if(box2->collider.dims.x>10.f){
                info.p_contact = {posx1, posy1};
            }
            else { //cas de base box to box
                info.p_contact.y=posy2+((posy1-posy2)/2);
                (posx1>posx2) ? info.p_contact.x=posx2+((posx1-posx2)/2) :info.p_contact.x=posx1+((posx2-posx1)/2) ;
            }
        }
        return true;
    }
    else
    {
        return false;
    }
}


//test collision circle to circle
bool collisionCircle2Circle(RigidBody * circle1, RigidBody * circle2, CollisionInfo & info){
    f32 rayon1 = circle1->collider.radius;
    f32 rayon2 = circle2->collider.radius;
    f32 distx=pow(circle1->position.x-circle2->position.x,2);
    f32 disty=pow(circle1->position.y-circle2->position.y,2);
    f32 dist=sqrt(distx+disty);

    if ((rayon1+rayon2)>dist){
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
        return false;
    }
}


//test collision box to circle
bool collisionBox2Circle(RigidBody * box, RigidBody * circle, CollisionInfo & info){
    f32 rayon = circle->collider.radius;
    f32 rayon2 = rayon * rayon;
    f32 by = circle->position.y;
    f32 bx = circle->position.x;
    vec2 taille = box->collider.dims;
    f32 minAy = box->position.y - taille.y/2;
    f32 minAx = box->position.x - taille.x/2;
    f32 maxAx = box->position.x + taille.x/2;
    f32 maxAy = box->position.y + taille.y/2;

    f32 minBy = circle->position.y - circle->collider.radius;
    f32 minBx = circle->position.x - circle->collider.radius;
    f32 maxBx = circle->position.x + circle->collider.radius;
    f32 maxBy = circle->position.y + circle->collider.radius;

    f32 posx1 = box->position.x;
    f32 posy1 = box->position.y;
    f32 posx2 = circle->position.x;
    f32 posy2 = circle->position.y;
    f32 penetrationX;
    f32 penetrationY;

    f32 py;
    f32 px;
    f32 p;
    f32 pmin=std::numeric_limits<float>::max();

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

    //distance du p trouve avec le cercle
    vec2 tposcircle = {px - bx,py-by};
    f32 norme2 = tposcircle.x*tposcircle.x + tposcircle.y* tposcircle.y;

    if (norme2 < rayon2) {
        info.rb1 = box;
        info.rb2 = circle;
        info.p_contact = {px,py};
        info.dp = sqrt(rayon2 - norme2); // rayon du cercle moins la distance entre le point(du carré) le plus proche du centre du cercle et le centre du cercle

        //calcul penetration en X et Y
        if ((posx1 > posx2))
            penetrationX = (maxBx - minAx);
        else
            penetrationX = (maxAx - minBx);

        if ((posy1 > posy2))
            penetrationY = (maxBy - minAy);
        else
            penetrationY = (maxAy - minBy);

        (penetrationX<penetrationY) ? info.dp = penetrationX : info.dp = penetrationY;

        if ((penetrationX<penetrationY) && posx2 > posx1 ){
            info.norm = {1,0};
        }
        else if ( (penetrationX<penetrationY) && posx1 > posx2 ){
            info.norm = {-1,0};
        }
        else if ((penetrationY<penetrationX)  && posy2 > posy1 ){
            info.norm = {0,1};
        }
        else if ( (penetrationY<penetrationX) && posy1 > posy2 ){
            info.norm = {0,-1};
        }

        return true;
    }
    else
        return false;
}


