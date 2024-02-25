#include "stdafx.h"
#include "HPBarSpriteComponent.h"
#include "GameObject.h"
#include "GameObjectManager.h"
#include "CharacterComponent.h"


HPBarSpriteComponent::HPBarSpriteComponent(const GameObject& parent) : IRenderComponent(parent)
{
    const auto& playerGameObj  = GameObjectManager::getInstance().getGameObjectById("player1");
    const auto& playerCharComp = playerGameObj->getComponentByType<CharacterComponent>();
    
    hpBarSize   = sf::Vector2i(416, 34);
    segmentSize = sf::Vector2i(hpBarSize.x / playerCharComp->getMaxHealth(), hpBarSize.y);
}

void HPBarSpriteComponent::update(float deltaTime)
{
    sprite->setPosition(m_parent.getPosition());
    sprite->setScale(m_parent.getScale());
    sprite->setRotation(m_parent.getRotation());

    const auto& playerGameObj  = GameObjectManager::getInstance().getGameObjectById("player1");
    const auto& playerCharComp = playerGameObj->getComponentByType<CharacterComponent>();

    int         segmentIndex = playerCharComp->getMaxHealth() - playerCharComp->getCurrentHealth();
    sf::IntRect textureRect(segmentIndex * segmentSize.x, 0, segmentSize.x, segmentSize.y);
    sprite->setTextureRect(textureRect);
}

void HPBarSpriteComponent::setSprite(std::shared_ptr<sf::Sprite> sprite)
{
    this->sprite = sprite;
}

std::shared_ptr<sf::Sprite> HPBarSpriteComponent::getSprite() const
{
    return sprite;
}

void HPBarSpriteComponent::draw(sf::RenderWindow& renderWindow)
{
    if (sprite)
    {
        renderWindow.draw(*sprite);
    }
}

bool HPBarSpriteComponent::init()
{
    return true;
}

int HPBarSpriteComponent::getRenderOrder() const
{
    return renderOrder;
}

void HPBarSpriteComponent::setRenderOrder(int order)
{
    renderOrder = order;
}

bool HPBarSpriteComponent::isVisible() const
{
    return visible;
}

void HPBarSpriteComponent::setVisible(bool isVisible)
{
    visible = isVisible;
}
