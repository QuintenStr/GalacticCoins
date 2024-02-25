#pragma once
#include "IComponent.h"

class GameObject;

class Component : public IComponent
{
public:
    Component(const GameObject& parent);
    const GameObject& getParent()
    {
        return m_parent;
    }

    virtual ~Component()
    {
    }

protected:
    const GameObject& m_parent;
};
