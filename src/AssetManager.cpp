#include "stdafx.h"

#include "AssetManager.h"

std::unordered_map<std::string, std::shared_ptr<sf::Texture>> AssetManager::Textures;
std::unordered_map<std::string, std::shared_ptr<sf::Sound>>   AssetManager::Sounds;
std::unordered_map<std::string, std::shared_ptr<sf::Music>>   AssetManager::Music;
std::unordered_map<std::string, std::shared_ptr<sf::Font>>    AssetManager::Fonts;
std::unordered_map<std::string, std::shared_ptr<sf::Texture>> AssetManager::m_tileSetTexture;
std::unordered_map<std::string, std::shared_ptr<sf::SoundBuffer>> SoundBuffers;

void AssetManager::LoadTexture(const std::string& name, const std::string& fileName)
{
    if (Textures.find(name) != Textures.end())
        return;

    auto texture = std::make_shared<sf::Texture>();
    if (texture->loadFromFile(fileName))
    {
        Textures[name] = texture;
    }
}

void AssetManager::LoadSound(const std::string& name, const std::string& fileName)
{
    if (Sounds.find(name) != Sounds.end())
        return;

    auto soundBuffer = std::make_shared<sf::SoundBuffer>();
    if (soundBuffer->loadFromFile(fileName))
    {
        SoundBuffers[name] = soundBuffer;
        auto sound   = std::make_shared<sf::Sound>(*soundBuffer);
        sound->setBuffer(*soundBuffer);
        Sounds[name] = sound;
    }
}

void AssetManager::LoadMusic(const std::string& name, const std::string& fileName)
{
    if (Music.find(name) != Music.end())
        return;

    auto music = std::make_shared<sf::Music>();
    if (music->openFromFile(fileName))
    {
        Music[name] = music;
    }
}

void AssetManager::LoadFont(const std::string& name, const std::string& fileName)
{
    if (Fonts.find(name) != Fonts.end())
        return;

    auto font = std::make_shared<sf::Font>();
    if (font->loadFromFile(fileName))
    {
        Fonts[name] = font;
    }
}

void AssetManager::LoadTileSetTexture(const std::string& name, const std::string& fileName)
{
    if (m_tileSetTexture.find(name) != m_tileSetTexture.end())
        return;

    auto texture = std::make_shared<sf::Texture>();
    if (!texture->loadFromFile(fileName))
    {
        sf::err() << "Could not load texture " << fileName << std::endl;
    }
    std::cout << "Loaded tile set texture " << name << " " << fileName << std::endl;

    m_tileSetTexture[name] = texture;
}
