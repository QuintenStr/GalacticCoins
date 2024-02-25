#pragma once

#include "DebugDraw.hpp"
#include "Game.hpp"
#include "GameObjectManager.h"
#include "GameStateManager.h"
#include "InputManager.h"
#include "PhysicsManager.h"
#include "RenderManager.h"
#include "AsteroidManager.h"

class GameState
{
protected:
    mmt_gd::Game*      m_game              = nullptr;
    mmt_gd::DebugDraw& m_debugDraw         = mmt_gd::DebugDraw::getInstance();
    GameObjectManager& m_gameObjectManager = GameObjectManager::getInstance();
    GameStateManager&  m_gameStateManager  = GameStateManager::getInstance();
    InputManager&      m_inputManager      = InputManager::getInstance();
    PhysicsManager&    m_physicsManager    = PhysicsManager::getInstance();
    RenderManager&     m_renderManager     = RenderManager::getInstance();
    AsteroidManager&   m_asteroidManager     = AsteroidManager::getInstance();

public:
    explicit GameState(mmt_gd::Game* game);

    virtual ~GameState() = default;

    virtual bool init()                           = 0;
    virtual void exit()                           = 0;
    virtual void update(float deltaTime)          = 0;
    virtual void render(sf::RenderWindow& window) = 0;
};