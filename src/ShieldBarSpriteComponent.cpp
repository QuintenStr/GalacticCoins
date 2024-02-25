#include "stdafx.h"
#include "ShieldBarSpriteComponent.h"
#include "stdafx.h"
#include "GameObject.h"
#include "GameObjectManager.h"
#include "CharacterComponent.h"
#include "MovementComponent.h"
#include "HumanController.h"


ShieldBarSpriteComponent::ShieldBarSpriteComponent(const GameObject& parent) : IRenderComponent(parent)
{
    const auto& playerGameObj  = GameObjectManager::getInstance().getGameObjectById("player1");
    const auto& playerCharComp = playerGameObj->getComponentByType<CharacterComponent>();

    hpBarSize   = sf::Vector2i(520, 34);
    segmentSize = sf::Vector2i(hpBarSize.x / playerCharComp->getMaxHealth(), hpBarSize.y);
}

void ShieldBarSpriteComponent::update(float deltaTime)
{
    sprite->setPosition(m_parent.getPosition());
    sprite->setScale(m_parent.getScale());
    sprite->setRotation(m_parent.getRotation());

    const auto& playerGameObj  = GameObjectManager::getInstance().getGameObjectById("player1");


    auto& movementComp   = playerGameObj->getComponentByType<MovementComponent>();
    auto&       controllerComp = movementComp->getController();

    std::shared_ptr<HumanController> controllerCompHuman = std::dynamic_pointer_cast<HumanController>(controllerComp);


    float timer = controllerCompHuman->getShieldTimer();

    int segmentIndex;
    if (timer > 5)
        segmentIndex = 0;
    else if (timer > 3.75)
        segmentIndex = 1;
    else if (timer > 2.5)
        segmentIndex = 2;
    else if (timer > 1.25)
        segmentIndex = 3;
    else
        segmentIndex = 4;

    sf::IntRect textureRect(segmentIndex * 104, 0, 104, 34);
    sprite->setTextureRect(textureRect);
}

void ShieldBarSpriteComponent::setSprite(std::shared_ptr<sf::Sprite> sprite)
{
    this->sprite = sprite;
}

std::shared_ptr<sf::Sprite> ShieldBarSpriteComponent::getSprite() const
{
    return sprite;
}

void ShieldBarSpriteComponent::draw(sf::RenderWindow& renderWindow)
{
    if (sprite)
    {
        renderWindow.draw(*sprite);
    }
}

bool ShieldBarSpriteComponent::init()
{
    return true;
}

int ShieldBarSpriteComponent::getRenderOrder() const
{
    return renderOrder;
}

void ShieldBarSpriteComponent::setRenderOrder(int order)
{
    renderOrder = order;
}

bool ShieldBarSpriteComponent::isVisible() const
{
    return visible;
}

void ShieldBarSpriteComponent::setVisible(bool isVisible)
{
    visible = isVisible;
}
