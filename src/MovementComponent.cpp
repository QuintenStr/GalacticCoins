#include "stdafx.h"

#include "MovementComponent.h"

MovementComponent::MovementComponent(const GameObject& parent) : Component(parent)
{
}

MovementComponent::~MovementComponent()
{
}

void MovementComponent::setStrategy(std::shared_ptr<IController> newController)
{
    controller = std::move(newController);
}

void MovementComponent::update(float deltaTime)
{
    if (controller)
    {
        controller->update(deltaTime);
    }
}

bool MovementComponent::init()
{
    return true;
}

std::shared_ptr<IController> MovementComponent::getController()
{
    return controller;
}
