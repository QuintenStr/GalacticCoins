#pragma once
#include "IComponent.h"
#include "IRenderComponent.h"

class GameObject : public sf::Transformable
{
public:
    GameObject(const std::string& id, const std::string& type);
    void                                     draw(sf::RenderWindow& window);
    void                                     update(float deltaTime);
    bool                                     init();
    void                                     addComponent(const std::shared_ptr<IComponent>& component);
    std::string                              getId();
    std::string                              getType();
    std::vector<std::shared_ptr<IComponent>> getComponents();

    template <typename T>
    std::shared_ptr<T> getComponentByType()
    {
        for (const auto& component : m_components)
        {
            std::shared_ptr<T> derived = std::dynamic_pointer_cast<T>(component);
            if (derived)
            {
                return derived;
            }
        }

        return nullptr;
    }

    template <typename T>
    void removeComponentsOfType()
    {
        auto newEnd = std::remove_if(m_components.begin(),
                                     m_components.end(),
                                     [](const std::shared_ptr<IComponent>& component)
                                     { return dynamic_cast<T*>(component.get()) != nullptr; });

        m_components.erase(newEnd, m_components.end());
    }

private:
    std::vector<std::shared_ptr<IComponent>> m_components;
    std::string                              m_id;
    std::string                              m_type;
};
