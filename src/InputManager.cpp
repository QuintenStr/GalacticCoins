#include "stdafx.h"

#include "InputManager.h"

InputManager& InputManager::getInstance()
{
    static InputManager instance;
    return instance;
}

InputManager::InputManager()
{
}

void InputManager::updateKeyStates()
{
    prevKeyStates = keyStates;

    for (int i = 0; i < sf::Keyboard::KeyCount; ++i)
    {
        keyStates[i] = sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(i));
    }
}

bool InputManager::isKeyDown(sf::Keyboard::Key keyCode)
{
    return keyStates[keyCode];
}

bool InputManager::isKeyUp(sf::Keyboard::Key keyCode)
{
    return !keyStates[keyCode];
}

bool InputManager::isKeyPressed(sf::Keyboard::Key keyCode)
{
    return keyStates[keyCode] && !prevKeyStates[keyCode];
}

bool InputManager::isKeyReleased(sf::Keyboard::Key keyCode)
{
    return !keyStates[keyCode] && prevKeyStates[keyCode];
}

bool InputManager::isKeyDown(const std::string& action, int playerIdx)
{
    auto it = keyBindings.find(action);
    if (it != keyBindings.end())
    {
        auto playerIt = it->second.find(playerIdx);
        if (playerIt != it->second.end())
        {
            return isKeyDown(playerIt->second);
        }
    }
    return false;
}

bool InputManager::isKeyUp(const std::string& action, int playerIdx)
{
    return !isKeyDown(action, playerIdx);
}

bool InputManager::isKeyPressed(const std::string& action, int playerIdx)
{
    auto it = keyBindings.find(action);
    if (it != keyBindings.end())
    {
        auto playerIt = it->second.find(playerIdx);
        if (playerIt != it->second.end())
        {
            return isKeyPressed(playerIt->second);
        }
    }
    return false;
}

bool InputManager::isKeyReleased(const std::string& action, int playerIdx)
{
    auto it = keyBindings.find(action);
    if (it != keyBindings.end())
    {
        auto playerIt = it->second.find(playerIdx);
        if (playerIt != it->second.end())
        {
            return isKeyReleased(playerIt->second);
        }
    }
    return false;
}

void InputManager::bind(const std::string& name, sf::Keyboard::Key keyCode, int playerIdx)
{
    keyBindings[name][playerIdx] = keyCode;
}

void InputManager::unbind(const std::string& name, int playerIdx)
{
    auto it = keyBindings.find(name);
    if (it != keyBindings.end())
    {
        it->second.erase(playerIdx);
    }
}