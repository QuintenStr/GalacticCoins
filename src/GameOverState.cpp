#include "stdafx.h"
#include "GameOverState.h"
#include "DebugDraw.hpp"
#include "Game.hpp"
#include "TGUI/TGUI.hpp"

GameOverState::GameOverState(mmt_gd::Game* game) : GameState(game)
{
}


bool GameOverState::init()
{
    m_game->getGui().loadWidgetsFromFile("../assets/gameovermenu.txt");

    // exit btn
    if (const auto btn = std::dynamic_pointer_cast<tgui::Button>(m_game->getGui().get("btnExit")))
    {
        btn->onClick([&] { this->exit(); });
    }
    return true;
}


void GameOverState::exit()
{
    m_game->getGui().removeAllWidgets();
    m_game->shutdown();
}

void GameOverState::update(float deltaTime)
{
}

void GameOverState::render(sf::RenderWindow& window)
{
    m_game->getGui().draw();
}
