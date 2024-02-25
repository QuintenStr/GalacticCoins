#include "stdafx.h"

#include "Game.hpp"

#include "DebugDraw.hpp"
#include "GameStateManager.h"
#include "MainState.h"
#include "LoadManager.h"

#include <iostream>
#include <sstream>
#include "StartMenuState.h"
#include "CharacterComponent.h"


namespace mmt_gd
{

void Game::run()
{
    if (!init())
    {
        return;
    }

    m_gui.setWindow(m_window);

    StartMenuState startMenuState(this);
    GameStateManager::getInstance().registerState("StartMenuState", std::make_shared<StartMenuState>(startMenuState));
    startMenuState.init();

    MainState mainState(this);
    GameStateManager::getInstance().registerState("MainState", std::make_shared<MainState>(mainState));

    GameStateManager::getInstance().setState("StartMenuState");
    while (m_window.isOpen())
    {
        sf::Event event{};
        while (m_window.pollEvent(event))
        {
            m_gui.handleEvent(event);

            if (event.type == sf::Event::Closed)
            {
                shutdown();
                m_window.close();
                return;
            }
        }

        update();
        draw();
    }

    shutdown();
}

bool Game::init()
{
    GameStateManager::getInstance().setGamePtr(this);

    LoadManager::getInstance().loadMap(m_resourcePath, "game.tmj", sf::Vector2f());

    m_debugDraw = &DebugDraw::getInstance();
    m_debugDraw->setEnabled(true);
    m_window.create(sf::VideoMode(m_config.m_resolution.x, m_config.m_resolution.y), m_config.m_windowName);

    return true;
}

void Game::update()
{
    static sf::Clock clock;
    const auto       deltaTime        = clock.restart();
    const auto       deltaTimeSeconds = deltaTime.asSeconds();

    m_debugDraw->update(deltaTimeSeconds);
    GameStateManager::getInstance().update(deltaTimeSeconds);

    std::ostringstream ss;
    m_fps.update();
    ss << m_config.m_windowName << " | FPS: " << m_fps.getFps();

    m_window.setTitle(ss.str());
}

void Game::draw()
{
    GameStateManager::getInstance().render(m_window);
    m_debugDraw->draw(m_window);
    m_window.display();
}

void Game::shutdown()
{
    // TO DO
    // Clean up any resources and managers
    // You may need to add more cleanup steps depending on your application

    // Shut down the game state manager
    //GameStateManager::getInstance().clear(); // Clear registered states

    // Shut down any other managers or systems that need cleanup
    // Example: LoadManager::getInstance().unloadAll();

    m_window.close();
    std::cout << "Shutting down..." << std::endl;
};
} // namespace mmt_gd
