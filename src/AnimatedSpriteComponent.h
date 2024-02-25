#pragma once
#include "IRenderComponent.h"

#include <SFML/Graphics.hpp>
#include <vector>

class AnimatedSpriteComponent : public IRenderComponent
{
public:
    AnimatedSpriteComponent(const GameObject& parent);

    void setTexture(std::shared_ptr<sf::Texture> texture);
    void setAnimationDetails(int frameCount, int frameWidth, int frameHeight, float frameDuration);

    void update(float deltaTime) override;
    void initialPositionUpdate();
    void draw(sf::RenderWindow& renderWindow) override;

    int  getRenderOrder() const;
    void setRenderOrder(int order);

    bool isVisible() const;
    void setVisible(bool isVisible);

    bool init() override;

private:
    std::shared_ptr<sf::Sprite> sprite;
    std::vector<sf::IntRect>    frames;
    float                       currentFrameTime;
    float                       frameDuration;
    int                         currentFrame;
    int                         frameCount;
    int                         renderOrder;
    bool                        visible;
};