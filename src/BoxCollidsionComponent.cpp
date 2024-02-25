#include "stdafx.h"
#include "BoxCollidsionComponent.h"

BoxCollisionComponent::BoxCollisionComponent(GameObject& parent, RigidBodyComponent& belongsTo) :
Component(parent),
m_belongsTo(belongsTo)
{
}

BoxCollisionComponent::~BoxCollisionComponent()
{
}


bool BoxCollisionComponent::init()
{
    return true;
}

void BoxCollisionComponent::update(float deltaTime)
{
}
