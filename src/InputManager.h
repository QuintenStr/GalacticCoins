#pragma once

class InputManager
{
public:
    static InputManager& getInstance();

    bool isKeyDown(sf::Keyboard::Key keyCode);
    bool isKeyUp(sf::Keyboard::Key keyCode);
    bool isKeyPressed(sf::Keyboard::Key keyCode);
    bool isKeyReleased(sf::Keyboard::Key keyCode);

    bool isKeyDown(const std::string& action, int playerIdx);
    bool isKeyUp(const std::string& action, int playerIdx);
    bool isKeyPressed(const std::string& action, int playerIdx);
    bool isKeyReleased(const std::string& action, int playerIdx);

    void bind(const std::string& name, sf::Keyboard::Key keyCode, int playerIdx);
    void unbind(const std::string& name, int playerIdx);
    void updateKeyStates();

private:
    InputManager();
    ~InputManager() = default;

    InputManager(const InputManager&)            = delete;
    InputManager& operator=(const InputManager&) = delete;

    std::unordered_map<std::string, std::unordered_map<int, sf::Keyboard::Key>> keyBindings;
    std::unordered_map<int, bool>                                               keyStates;
    std::unordered_map<int, bool>                                               prevKeyStates;
};
