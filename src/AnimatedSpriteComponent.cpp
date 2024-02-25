#include "stdafx.h"
#include "AnimatedSpriteComponent.h"
#include "GameObject.h"

AnimatedSpriteComponent::AnimatedSpriteComponent(const GameObject& parent) :
IRenderComponent(parent),
currentFrameTime(0.0f),
frameDuration(0.0f),
currentFrame(0),
frameCount(0)
{
    sprite = std::make_shared<sf::Sprite>();
}

void AnimatedSpriteComponent::setTexture(std::shared_ptr<sf::Texture> texture)
{
    sprite->setTexture(*texture);
    if (!frames.empty())
    {
        sprite->setTextureRect(frames[0]);
    }
}

void AnimatedSpriteComponent::setAnimationDetails(int frameCount, int frameWidth, int frameHeight, float frameDuration)
{
    this->frameCount    = frameCount;
    this->frameDuration = frameDuration;
    frames.clear();

    for (int i = 0; i < frameCount; ++i)
    {
        frames.emplace_back(sf::IntRect(frameWidth * i, 0, frameWidth, frameHeight));
    }
    if (frameCount > 0)
    {
        sprite->setTextureRect(frames[0]);
    }
}

void AnimatedSpriteComponent::update(float deltaTime)
{
    currentFrameTime += deltaTime;
    if (currentFrameTime >= frameDuration)
    {
        currentFrameTime = 0.0f;
        currentFrame     = (currentFrame + 1) % frameCount;
        sprite->setTextureRect(frames[currentFrame]);
    }

    sprite->setPosition(m_parent.getPosition());
    sprite->setScale(m_parent.getScale());
    sprite->setRotation(m_parent.getRotation());
}

void AnimatedSpriteComponent::initialPositionUpdate()
{
    sprite->setPosition(m_parent.getPosition());
    sprite->setScale(m_parent.getScale());
    sprite->setRotation(m_parent.getRotation());
}

void AnimatedSpriteComponent::draw(sf::RenderWindow& renderWindow)
{
    if (sprite)
    {
        renderWindow.draw(*sprite);
    }
}

bool AnimatedSpriteComponent::init()
{
    return true;
}

int AnimatedSpriteComponent::getRenderOrder() const
{
    return renderOrder;
}

void AnimatedSpriteComponent::setRenderOrder(int order)
{
    renderOrder = order;
}

bool AnimatedSpriteComponent::isVisible() const
{
    return visible;
}

void AnimatedSpriteComponent::setVisible(bool isVisible)
{
    visible = isVisible;
}
