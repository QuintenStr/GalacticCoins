#include "stdafx.h"
#include "SpriteComponent.h"
#include "GameObject.h"

SpriteComponent::SpriteComponent(const GameObject& parent) : IRenderComponent(parent)
{
}

void SpriteComponent::setSprite(std::shared_ptr<sf::Sprite> sprite)
{
    this->sprite = sprite;
}

std::shared_ptr<sf::Sprite> SpriteComponent::getSprite() const
{
    return sprite;
}

void SpriteComponent::draw(sf::RenderWindow& renderWindow)
{
    if (sprite)
    {
        renderWindow.draw(*sprite);
    }
}

bool SpriteComponent::init()
{
    return true;
}

void SpriteComponent::update(float deltaTime)
{
    sprite->setPosition(m_parent.getPosition());
    sprite->setScale(m_parent.getScale());
    sprite->setRotation(m_parent.getRotation());
}

int SpriteComponent::getRenderOrder() const
{
    return renderOrder;
}

void SpriteComponent::setRenderOrder(int order)
{
    renderOrder = order;
}

bool SpriteComponent::isVisible() const
{
    return visible;
}

void SpriteComponent::setVisible(bool isVisible)
{
    visible = isVisible;
}
