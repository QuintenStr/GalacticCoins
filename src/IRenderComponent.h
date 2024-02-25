#pragma once
#include "Component.h"

class IRenderComponent : public Component
{
public:
    IRenderComponent(const GameObject& parent) : Component(parent)
    {
    }

    virtual ~IRenderComponent()
    {
    }

    virtual void draw(sf::RenderWindow& renderWindow) = 0;
};
