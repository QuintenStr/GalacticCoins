#pragma once
#include "GameState.h"
#include "TGUI/TGUI.hpp"

class StartMenuState : public GameState
{
public:
    StartMenuState(mmt_gd::Game* game);
    bool init() override;
    void exit() override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;
};
