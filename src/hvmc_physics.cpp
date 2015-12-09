#include "hvmc_physics.h"
#include "iostream"
#include "math.h"


void RigidBody::Update( f32 dt )
{
   /* vec2 acc = im * forces;
    velocity += dt*acc;
    position += dt*velocity;
    rotation += rotation*dt;
    ApplyVelocity(dt);*/
}

void RigidBody::ApplyForce( vec2 const& f )
{
    forces+=f;
}

void RigidBody::IntegrateForces(f32 dt){
    // linearVelocity+=a*dt;
    // angularVelocity += theta*dt;
    vec2 acc = im * forces;
    velocity += acc * dt;
    f32 theta = im * torque;
    angularVelocity += theta*dt;
}

void RigidBody::IntegrateVelocities(f32 dt)
{
    //position+=v*dt; rotation+=w*dt;
    position += velocity*dt;
    rotation += angularVelocity*dt;
}

void RigidBody::ApplyVelocity( f32 dt )
{
    position+=velocity*dt;
    rotation+=rotation*dt;
}

void RigidBody::ApplyImpulse( vec2 const& impulse, vec2 const& contactVector )
{
    //velocity+=(velocity+impulse),contactVector;
    velocity += impulse*im;

    angularVelocity += Cross((contactVector-position),impulse*im);
}

void RigidBody::SetKinematic()
{
    I = iI = m = im = 0.f;
}

bool PhysicsSystem::Init()
{
    gravity = vec2{ 0.f, -9.81f };

    return true;
}

void PhysicsSystem::Cleanup()
{
    rigidBodies.clear();
}

RigidBody* PhysicsSystem::AddSphere( vec2 const& pos, f32 radius )
{
    RigidBody* body = new RigidBody;
    
    body->forces = { 0.f, 0.f };
    body->im = 1.f; // 1 kg
    body->m = 1.f;
    body->iI = 1.f;
    body->position = pos;
    body->velocity = { 0.f, 0.f };
    body->e = 0.7;

    body->collider.type = RIGID_BODY_SPHERE;
    body->collider.radius = radius;

    rigidBodies.push_back( body );
    return body;
}

RigidBody* PhysicsSystem::AddBox( vec2 const& pos, vec2 const& dims )
{
    RigidBody* body = new RigidBody; 
    
    body->forces = { 0.f, 0.f };
    
    body->im = 1.f; // 1 kg
    body->m = 1.f;
    body->position = pos;
    body->velocity = { 0.f, 0.f };
    body->e = 0.3;
    
    body->collider.type = RIGID_BODY_BOX;
    body->collider.dims = dims;

    rigidBodies.push_back( body );
    return body;
}

RigidBody* PhysicsSystem::AddWall( vec2 const& pos, vec2 const& dims )
{
    RigidBody* body = new RigidBody;

    body->im = 0.f;
    body->position = pos;

    //body->collider.type = RIGID_BODY_WALL;
    body->collider.type = RIGID_BODY_BOX;
    body->collider.dims = dims;

    body->velocity = { 0.f, 0.f };
    body->angularVelocity = 0;

    rigidBodies.push_back( body );
    return body;
}

void CollisionInfo::Solve()
{
    f32 e;
    if (rb1->e < rb2->e) e = rb1->e; else e = rb2->e;

    vec2 rA, rB;
    rA = p_contact - rb1->position;
    rB = p_contact - rb2->position;

    vec2 vrel= (rb2->velocity + Cross(rb2->angularVelocity,rB)) - (rb1->velocity + Cross(rb1->angularVelocity,rA));

    if (Dot(vrel,norm) <= 0) //positif, pas d'impulsions aux objets
    {
        f32 J = (-(1 + e) * (Dot(vrel,norm))) / (rb1->im + rb2->im + rb1->iI*(Cross(rA,norm)) + rb2->iI*(Cross(rB,norm)));

        vec2 JA = -J*norm;
        vec2 JB = J*norm;

        rb1->ApplyImpulse(JA,p_contact);
        rb2->ApplyImpulse(JB,p_contact);
    }
}

void CollisionInfo::correction(){
    const f32 seuil = 0.01f;
    const f32 p = 0.5f;
    vec2 correction = (std::max(dp - seuil,0.0f) / (rb1->im + rb2->im)) * p *norm;

    rb1->position = rb1->position - (rb1->im * correction);
    rb2->position = rb2->position + (rb2->im * correction);
}

void PhysicsSystem::Update( f32 dt )
{    
    //add gravity
    for (RigidBody* rb : rigidBodies){
        rb->ApplyForce(rb->m * gravity);
        //rb->ApplyImpulse({ 0.f, -9.81f },{ 0.f, -9.81f });

        //rb->Update(dt);
        //rb->forces = {0.0,0.0};
    }
    std::vector<CollisionInfo> collisions;

    u32 count = rigidBodies.size();

    //generate contact infos
    for (u32 i = 0; i< count -1; i++){
        for (u32 j = i+1; j < count; j++){
            CollisionInfo info;
            RigidBody * rb1 = rigidBodies[i];
            RigidBody * rb2 = rigidBodies[j];

            if (collider(rb1,rb2,info)){
                collisions.push_back(info); //solveur
            }
        }
    }

    //Integrate Forces
    for (RigidBody* rb : rigidBodies){
        rb->IntegrateForces(dt);
        // linearVelocity+=a*dt;
        // angularVelocity += theta*dt;
    }

    // solve contacts
    for (auto collision : collisions){
        collision.Solve();
        collision.correction();
    }

    //integrate velocities
    for(auto &rb : rigidBodies ){
        rb->IntegrateVelocities(dt); //position+=v*dt; rotation+=w*dt;
    }

    //clear forces
    for( auto & rb : rigidBodies ){
        rb->forces = {0.0,0.0};
        rb->torque = 0.0;
    }
}


