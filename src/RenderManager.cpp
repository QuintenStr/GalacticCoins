#include "stdafx.h"

#include "RenderManager.h"

#include "SpriteComponent.h"
#include "TileLayerComponent.h"
#include "GameObjectManager.h"
#include "AnimatedSpriteComponent.h"
#include "HPBarSpriteComponent.h"
#include "ShieldBarSpriteComponent.h"
#include "TextComponent.h"

RenderManager& RenderManager::getInstance()
{
    static RenderManager instance;
    return instance;
}
struct CustomMap
{
    int               id;
    IRenderComponent* renderComponent;
    bool              visible;

    CustomMap(int i, IRenderComponent* rc, bool isVisible) : id(i), renderComponent(rc), visible(isVisible)
    {
    }

    bool operator<(const CustomMap& other) const
    {
        return id < other.id;
    }
};

std::vector<CustomMap> myVector;

void RenderManager::render(sf::RenderWindow& window)
{
    window.clear();

    auto gameObjects = GameObjectManager::getInstance().getGameObjects();

    myVector.clear();

    for (const auto& gameObject : gameObjects)
    {
        for (const auto& component : gameObject->getComponents())
        {
            if (auto* tileLayer = dynamic_cast<TileLayerComponent*>(component.get()))
            {
                myVector.emplace_back(tileLayer->getRenderOrder(), tileLayer, tileLayer->isVisible());
            }
            else if (auto* spriteComponent = dynamic_cast<SpriteComponent*>(component.get()))
            {
                myVector.emplace_back(spriteComponent->getRenderOrder(), spriteComponent, spriteComponent->isVisible());
            }
            else if (auto* spriteComponent = dynamic_cast<AnimatedSpriteComponent*>(component.get()))
            {
                myVector.emplace_back(spriteComponent->getRenderOrder(), spriteComponent, spriteComponent->isVisible());
            }
            else if (auto* spriteComponent = dynamic_cast<HPBarSpriteComponent*>(component.get()))
            {
                myVector.emplace_back(spriteComponent->getRenderOrder(), spriteComponent, spriteComponent->isVisible());
            }
            else if (auto* spriteComponent = dynamic_cast<ShieldBarSpriteComponent*>(component.get()))
            {
                myVector.emplace_back(spriteComponent->getRenderOrder(), spriteComponent, spriteComponent->isVisible());
            }
            else if (auto* textComponent = dynamic_cast<TextComponent*>(component.get()))
            {
                myVector.emplace_back(textComponent->getRenderOrder(), textComponent, textComponent->isVisible());
            }
        }
    }

    std::sort(myVector.begin(), myVector.end());

    for (const auto& item : myVector)
    {
        if (item.visible)
        {
            item.renderComponent->draw(window);
        }
    }
}