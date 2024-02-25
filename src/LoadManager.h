#pragma once

class LoadManager
{
public:
    static LoadManager& getInstance();

    void loadMap(const fs::path& resourcePath, const fs::path& filename, const sf::Vector2f& offset);

    LoadManager(const LoadManager&)            = delete;
    LoadManager& operator=(const LoadManager&) = delete;

    std::unordered_map<std::string, std::shared_ptr<sf::Texture>> m_tileSetTexture;

private:
    LoadManager();
    fs::path LoadManager::m_resourcePath;
};
