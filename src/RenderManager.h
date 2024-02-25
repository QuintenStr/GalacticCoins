#pragma once

class RenderManager
{
public:
    static RenderManager& getInstance();

    void render(sf::RenderWindow& window);

private:
    RenderManager()  = default;
    ~RenderManager() = default;

    RenderManager(const RenderManager&)            = delete;
    RenderManager& operator=(const RenderManager&) = delete;
};