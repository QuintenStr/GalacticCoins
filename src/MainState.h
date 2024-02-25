#pragma once
#include "GameState.h"

class MainState : public GameState
{
public:
    MainState(mmt_gd::Game* game);
    bool init() override;
    void exit() override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;
};
