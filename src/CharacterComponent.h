#pragma once
#include "Component.h"
#include "Game.hpp"

class CharacterComponent : public Component
{
public:
    CharacterComponent(const GameObject& parent);
    virtual ~CharacterComponent();

    void update(float deltaTime) override;
    bool init() override;

    void PickupCoin();
    int getCoinCount();

    void TakeDamage();
    int  getMaxHealth();
    int getCurrentHealth();

    void setIsShielded(bool isShielded);
    bool getIsShielded();

    void setGamePtr(mmt_gd::Game *game);

private:
    int  m_maxHealth;
    int  m_currentHealth;
    int  m_countPickedUpCoins;
    bool m_isDead;
    bool m_isShielded;
    mmt_gd::Game* m_game = nullptr;
};