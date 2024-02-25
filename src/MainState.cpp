#include "stdafx.h"

#include "MainState.h"
#include "DebugDraw.hpp"
#include "Game.hpp"

MainState::MainState(mmt_gd::Game* game) : GameState(game)
{
}


bool MainState::init()
{
    m_inputManager.bind("move_up", sf::Keyboard::W, 1);
    m_inputManager.bind("move_down", sf::Keyboard::S, 1);
    m_inputManager.bind("move_left", sf::Keyboard::A, 1);
    m_inputManager.bind("move_right", sf::Keyboard::D, 1);
    m_inputManager.bind("shield", sf::Keyboard::Space, 1);

    return true;
}


void MainState::exit()
{
}

void MainState::update(float deltaTime)
{
    m_inputManager.updateKeyStates();
    m_gameObjectManager.update(deltaTime);
    m_physicsManager.update(deltaTime);
    m_asteroidManager.update(deltaTime);
}

void MainState::render(sf::RenderWindow& window)
{
    m_renderManager.render(window);
}
