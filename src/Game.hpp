#pragma once

#include "FPS.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <string>
#include <TGUI/Backend/SFML-Graphics.hpp>

namespace mmt_gd
{
class InputManager;
class DebugDraw;

class Game
{
public:
    struct Config
    {
        sf::Vector2i m_resolution{960, 640};
        std::string  m_windowName = "MMP";
    };

    Config& getConfig()
    {
        return m_config;
    }

    sf::RenderWindow& getWindow()
    {
        return m_window;
    }

    tgui::Gui& getGui()
    {
        return m_gui;
    }

    void run();
    void shutdown();

private:
    const fs::path m_resourcePath{"../assets"};

    bool init();
    void update();
    void draw();

    Config m_config;

    sf::RenderWindow m_window;

    InputManager* m_inputManager = nullptr;
    DebugDraw*    m_debugDraw    = nullptr;
    Fps           m_fps;
    tgui::Gui     m_gui;
};
} // namespace mmt_gd
