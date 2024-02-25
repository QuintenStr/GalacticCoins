#pragma once
#include "GameObject.h"

class IController
{
protected:
    GameObject& owner;

public:
    IController(GameObject& owner) : owner(owner)
    {
    }
    virtual ~IController()               = default;
    virtual void update(float deltaTime) = 0;
};