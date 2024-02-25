#include "stdafx.h"

#include "RigidBodyComponent.h"

RigidBodyComponent::RigidBodyComponent(const GameObject& parent) : Component(parent)
{
}

RigidBodyComponent::~RigidBodyComponent()
{
}

bool RigidBodyComponent::init()
{
    return true;
}

void RigidBodyComponent::update(float deltaTime)
{
    GameObject& objectToUpdate = const_cast<GameObject&>(m_parent);

    sf::Vector2f forces{};

    for (const auto& force : m_forces)
    {
        forces += force;
    }

    for (const auto& impulse : m_impulses)
    {
        forces += impulse;
    }

    m_impulses.clear();

    m_acceleration = forces * m_invMass;
    m_velocity += m_acceleration * deltaTime;

    objectToUpdate.move(m_velocity * deltaTime);
}
