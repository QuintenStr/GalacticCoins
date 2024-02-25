#include "stdafx.h"
#include "TileLayerComponent.h"
#include "GameObject.h"

TileLayerComponent::TileLayerComponent(const GameObject& parent) : IRenderComponent(parent)
{
}

bool compareSprites(std::shared_ptr<sf::Sprite> sprite1, std::shared_ptr<sf::Sprite> sprite2)
{
    return sprite1->getPosition().y < sprite2->getPosition().y ||
           (sprite1->getPosition().y == sprite2->getPosition().y && sprite1->getPosition().x < sprite2->getPosition().x);
}

void TileLayerComponent::addSprite(std::shared_ptr<sf::Sprite> sprite)
{
    sprites.push_back(sprite);
}

void TileLayerComponent::draw(sf::RenderWindow& renderWindow)
{
    // sort so we render from left to right, top to bottom
    std::sort(sprites.begin(), sprites.end(), compareSprites);

    for (const auto& sprite : sprites)
    {
        renderWindow.draw(*sprite);
    }
}

int TileLayerComponent::getRenderOrder() const
{
    return renderOrder;
}

void TileLayerComponent::setRenderOrder(int order)
{
    renderOrder = order;
}

bool TileLayerComponent::init()
{
    return true;
}

bool TileLayerComponent::isVisible() const
{
    return visible;
}

void TileLayerComponent::setVisible(bool isVisible)
{
    visible = isVisible;
}

void TileLayerComponent::update(float deltaTime)
{
}
