#pragma once

class GameObject;

class IComponent
{
public:
    virtual ~IComponent()
    {
    }

    virtual bool init()                  = 0;
    virtual void update(float deltaTime) = 0;
};
