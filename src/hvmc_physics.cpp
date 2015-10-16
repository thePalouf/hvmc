#include "hvmc_physics.h"

void RigidBody::Update( f32 dt )
{
    vec2 acc = im * forces;
    velocity += dt*acc;
    position += dt*velocity;
}

void RigidBody::ApplyForce( vec2 const& f )
{
    forces+=f;
}

void RigidBody::ApplyImpulse( vec2 const& impulse, vec2 const& contactVector )
{
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

    body->collider.type = RIGID_BODY_BOX;
    body->collider.dims = dims;

    rigidBodies.push_back( body );
    return body;
}

void PhysicsSystem::Update( f32 dt )
{    
    //add gravity
    for (RigidBody* rb : rigidBodies){
        rb->ApplyForce(rb->m * gravity);
        rb->Update(dt);
    }

    //generate contact infos
    /*for (RigidBody* rb1 : rigidBodies){
        for (RigidBody* rb2 : rigidBodies){

              CollisionInfo info;
              if ( Collider(a,b,info)
                collisions.push_back(info);

        }

         //Integrate Forces
         for (RigidBody* rb : rigidBodies){
            rb->IntegrateForces(dt); // linearVelocity+=a*dt;
                                     // angularVelocity += theta*dt;
         }
         //integrate velocities
         for(){
            rb.integratevelocities(dt); //position+=v*dt; rotation+=w*dt;
         }
         //clear forces
         for(){
             rb.forces = {0.0,0.0};
             rb.torque = 0.0;
         }

    }
    }*/
}

