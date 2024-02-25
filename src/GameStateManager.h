#pragma once
#include "Game.hpp"

class GameState;

class GameStateManager
{
private:
    std::unordered_map<std::string, std::shared_ptr<GameState>> states;
    std::shared_ptr<GameState>                                  currentState;

    GameStateManager() = default;
    mmt_gd::Game* m_game = nullptr;

public:
    static GameStateManager& getInstance()
    {
        static GameStateManager instance;
        return instance;
    }

    void                       registerState(std::string name, std::shared_ptr<GameState> state);
    void                       setState(std::string name);
    void                       update(float deltaTime);
    void                       render(sf::RenderWindow& window);
    std::shared_ptr<GameState> getCurrentState();
    void                       deleteState(const std::string& name);
    void setGamePtr(mmt_gd::Game* game);
    mmt_gd::Game* getGamePtr();

};