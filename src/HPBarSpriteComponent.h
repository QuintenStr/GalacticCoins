#pragma once
#include "IRenderComponent.h"

class HPBarSpriteComponent : public IRenderComponent
{
public:
    HPBarSpriteComponent(const GameObject& parent);

    void                        setSprite(std::shared_ptr<sf::Sprite> sprite);
    std::shared_ptr<sf::Sprite> getSprite() const;

    void draw(sf::RenderWindow& renderWindow) override;

    int  getRenderOrder() const;
    void setRenderOrder(int order);

    bool isVisible() const;
    void setVisible(bool isVisible);

    bool init() override;
    void update(float deltaTime) override;

private:
    std::shared_ptr<sf::Sprite> sprite;
    int                         renderOrder;
    bool                        visible;
    sf::Vector2i                hpBarSize;
    sf::Vector2i                segmentSize;
};