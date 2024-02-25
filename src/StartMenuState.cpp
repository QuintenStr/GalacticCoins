#include "stdafx.h"
#include "StartMenuState.h"
#include "stdafx.h"
#include "DebugDraw.hpp"
#include "Game.hpp"
#include "TGUI/TGUI.hpp"

StartMenuState::StartMenuState(mmt_gd::Game* game) : GameState(game)
{
}


bool StartMenuState::init()
{
    m_game->getGui().loadWidgetsFromFile("../assets/startmenu.txt");

    if (const auto btn = std::dynamic_pointer_cast<tgui::Button>(m_game->getGui().get("btnStart")))
    {
        btn->onClick([&manager = m_gameStateManager] { manager.setState("MainState"); });
    }
    return true;
}


void StartMenuState::exit()
{
    m_game->getGui().removeAllWidgets();
}

void StartMenuState::update(float deltaTime)
{
}

void StartMenuState::render(sf::RenderWindow& window)
{
    m_game->getGui().draw();
}
