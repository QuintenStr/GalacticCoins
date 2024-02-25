#pragma once
#include "Component.h"
#include "GameObject.h"

class RigidBodyComponent : public Component
{
public:
    RigidBodyComponent(const GameObject& parent);
    ~RigidBodyComponent();

    bool init() override;
    void update(float deltaTime) override;

    sf::Vector2f              m_acceleration;
    sf::Vector2f              m_velocity;
    std::vector<sf::Vector2f> m_forces;
    std::vector<sf::Vector2f> m_impulses;
    float                     m_invMass;
    float                     m_mass;
    float                     m_restitution;
};
