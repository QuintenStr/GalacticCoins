#pragma once
#include "RigidBodyComponent.h"

struct Manifold
{
    RigidBodyComponent* m_body1{};
    RigidBodyComponent* m_body2{};

    float        m_penetration{};
    sf::Vector2f m_normal;
};

class PhysicsManager
{
public:
    static PhysicsManager& getInstance()
    {
        static PhysicsManager instance;
        return instance;
    }

    void update(float deltaTime);
    void updateStep(float deltaTime);
    void getBodies();

    void findCollisions();

    void substepResolvecollision(Manifold man);

    void resolveCollisions();

    void positionalCorrection(Manifold man);


private:
    std::vector<RigidBodyComponent*> m_bodies;
    std::vector<Manifold>            m_manifolds;
    float                            m_accumulator = 0.f;

    PhysicsManager();
    ~PhysicsManager() = default;
};
