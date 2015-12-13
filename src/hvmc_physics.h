#ifndef HVMC_PHYSICS_H
#define HVMC_PHYSICS_H

#include <vector>
#include "hvmc_math.h"
#include "hvmc_collisions.h"

enum RigidBodyType
{
    RIGID_BODY_SPHERE,
    RIGID_BODY_BOX
    //RIGID_BODY_WALL /*added this to differenciate wall from others, walls don't MOVE !!!*/
};

//structure collision
struct CollisionInfo{
    RigidBody * rb1;
    RigidBody * rb2;
    vec2 norm;
    f32 dp;
    vec2 p_contact;

    void Solve();
    void correction(); //contre l'effet sable mouvant
};

struct Collider
{
    Collider() {}
    RigidBodyType type;

    union
    {
        f32 radius; // Sphere
        vec2 dims; // Box
    };
};

struct RigidBody
{
    RigidBody() {}
    ~RigidBody() {}
    
    void Update( f32 dt );
    
    void ApplyForce( vec2 const& force );
    void ApplyImpulse( vec2 const& impulse, vec2 const& contactVector );
    void ApplyVelocity( f32 dt );
    void IntegrateForces(f32 dt);
    void IntegrateVelocities(f32 dt);
    
    void SetKinematic();
    void SetGravityMode( int mode );
    
    f32 I = 0.f;  // inertia
    f32 iI = 0.f; // inverse inertia
    f32 m = 0.f;  // mass
    f32 im = 0.f; // inverse mass
    
    int gravityMode = 1;

    vec2 forces;
    vec2 position;
    vec2 velocity;

    f32 torque = 0.f;
    f32 rotation = 0.f; 
    f32 angularVelocity = 0.f;
    f32 e = 0.5;

    Collider collider;
};

struct PhysicsSystem
{
    bool Init();
    void Cleanup();
    
    void Update( f32 dt );
    
    RigidBody* AddSphere( vec2 const& pos, f32 radius );
    RigidBody* AddBox( vec2 const& pos, vec2 const& dims );
    RigidBody* AddWall( vec2 const& pos, vec2 const& dims );

    std::vector<RigidBody*> rigidBodies;
    vec2 gravity;
};

#endif

