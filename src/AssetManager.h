#pragma once

class AssetManager
{
public:
    static std::unordered_map<std::string, std::shared_ptr<sf::Texture>> Textures;
    static std::unordered_map<std::string, std::shared_ptr<sf::Sound>>   Sounds;
    static std::unordered_map<std::string, std::shared_ptr<sf::Music>>   Music;
    static std::unordered_map<std::string, std::shared_ptr<sf::Font>>    Fonts;
    static std::unordered_map<std::string, std::shared_ptr<sf::Texture>> m_tileSetTexture;

    static void LoadTexture(const std::string& name, const std::string& fileName);
    static void LoadSound(const std::string& name, const std::string& fileName);
    static void LoadMusic(const std::string& name, const std::string& fileName);
    static void LoadFont(const std::string& name, const std::string& fileName);
    static void LoadTileSetTexture(const std::string& name, const std::string& fileName);
};