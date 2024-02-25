#pragma once
#include "Component.h"

class RigidBodyComponent;

class BoxCollisionComponent : public Component
{
public:
    BoxCollisionComponent(GameObject& parent, RigidBodyComponent& belongsTo);

    ~BoxCollisionComponent();

    bool init() override;
    void update(float deltaTime) override;

    sf::FloatRect       m_shape;
    RigidBodyComponent& m_belongsTo;
};
