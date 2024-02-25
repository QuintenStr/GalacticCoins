#include "stdafx.h"
#include "CharacterComponent.h"
#include "GameOverState.h"

CharacterComponent::CharacterComponent(const GameObject& parent) :
Component(parent),
m_maxHealth(4),
m_currentHealth(4),
m_countPickedUpCoins(0),
m_isDead(false),
m_isShielded(false)
{
    init();
}

CharacterComponent::~CharacterComponent()
{
}


void CharacterComponent::update(float deltaTime)
{
    if (m_currentHealth <= 0)
    {
        m_isDead = true;

        GameOverState gameOverState(m_game);
        GameStateManager::getInstance().registerState("GameOverState", std::make_shared<GameOverState>(gameOverState));
        GameStateManager::getInstance().setState("GameOverState");
    }
}

bool CharacterComponent::init()
{
    m_game = GameStateManager::getInstance().getGamePtr();
    return true;
}

void CharacterComponent::PickupCoin()
{
    m_countPickedUpCoins++;
}

int CharacterComponent::getCoinCount()
{
    return m_countPickedUpCoins;
}

void CharacterComponent::TakeDamage()
{
    if (m_currentHealth <= 0)
    {
        std::cout << "Player is already dead." << std::endl;
        return;
    }

    m_currentHealth -= 1;
    std::cout << "Player hit asteroid, loss 1 hp" << std::endl;
    std::cout << "Current hp: " << m_currentHealth << std::endl;

    if (m_currentHealth <= 0)
    {
        m_isDead = true;
        // TODO GAMESTATEMANAGER SHOW END SCREEN
        std::cout << "Player died, end game" << std::endl;
    }
    else if (m_currentHealth > m_maxHealth)
    {
        std::cout << "Warning: Health is greater than max health." << std::endl;
    }
}

int CharacterComponent::getMaxHealth()
{
    return m_maxHealth;
}

int CharacterComponent::getCurrentHealth()
{
    return m_currentHealth;
}

void CharacterComponent::setIsShielded(bool isShielded)
{
    m_isShielded = isShielded;
}

bool CharacterComponent::getIsShielded()
{
    return m_isShielded;
}

void CharacterComponent::setGamePtr(mmt_gd::Game* game)
{
    m_game = game;
}
