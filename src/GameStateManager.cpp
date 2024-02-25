#include "stdafx.h"

#include "GameStateManager.h"

#include "GameState.h"

void GameStateManager::registerState(std::string name, std::shared_ptr<GameState> state)
{
    states[name] = state;
}

std::shared_ptr<GameState> GameStateManager::getCurrentState()
{
    return currentState;
}

void GameStateManager::deleteState(const std::string& name)
{
    auto it = states.find(name);
    if (it != states.end())
    {
        if (currentState == it->second)
        {
            currentState->exit();
            currentState = nullptr;
        }

        it->second->exit();

        states.erase(it);
    }
}

void GameStateManager::setGamePtr(mmt_gd::Game* game)
{
    m_game = game;
}

mmt_gd::Game* GameStateManager::getGamePtr()
{
    return m_game;
}

void GameStateManager::setState(std::string name)
{
    auto it = states.find(name);
    if (it != states.end())
    {
        if (currentState)
        {
            currentState->exit();
        }
        currentState = it->second;
        currentState->init();
    }
}

void GameStateManager::update(float deltaTime)
{
    if (currentState)
    {
        currentState->update(deltaTime);
    }
}

void GameStateManager::render(sf::RenderWindow& window)
{
    if (currentState)
    {
        currentState->render(window);
    }
}
