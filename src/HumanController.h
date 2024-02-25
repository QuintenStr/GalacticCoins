#pragma once
#include "IController.h"
#include "AnimatedSpriteComponent.h"

class HumanController : public IController
{
public:
    HumanController(GameObject& owner);
    void update(float deltaTime) override;

    void activateShield();

    void deactivateShield();

    void setOriginalSprite(std::shared_ptr<AnimatedSpriteComponent> originalSprite);
    void setShieldSprite(std::shared_ptr<AnimatedSpriteComponent> shieldSprite);

    float getShieldTimer();

private:
    float                                    m_shieldTimer       = 0.0f;
    float                                    m_activeShieldTimer = 0.0f;
    bool                                     m_shieldActive      = false;
    std::shared_ptr<AnimatedSpriteComponent> m_originalSprite;
    std::shared_ptr<AnimatedSpriteComponent> m_shieldSprite;
};
