#include "stdafx.h"

#include "GameObject.h"

//#include "RigidBodyComponent.h"

GameObject::GameObject(const std::string& id, const std::string& type) : m_id(id), m_type(type)
{
}

void GameObject::addComponent(const std::shared_ptr<IComponent>& component)
{
    m_components.push_back(component);
}

std::string GameObject::getId()
{
    return m_id;
}

std::string GameObject::getType()
{
    return m_type;
}

std::vector<std::shared_ptr<IComponent>> GameObject::getComponents()
{
    return m_components;
}

bool GameObject::init()
{
    for (const auto& component : m_components)
    {
        component->init();
    }
    return true;
}

void GameObject::draw(sf::RenderWindow& window)
{
    for (const auto& component : m_components)
    {
        if (const auto comp = std::dynamic_pointer_cast<IRenderComponent>(component))
        {
            comp->draw(window);
        }
    }
}

void GameObject::update(float deltaTime)
{
    for (const auto& component : m_components)
    {
        if (const auto comp = std::dynamic_pointer_cast<IComponent>(component))
        {
            comp->update(deltaTime);
        }
    }
}
