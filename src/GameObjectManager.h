#pragma once
#include "GameObject.h"

class GameObjectManager
{
public:
    static GameObjectManager& getInstance()
    {
        static GameObjectManager instance;
        return instance;
    }

    void                                            addGameObject(std::shared_ptr<GameObject> object);
    const std::vector<std::shared_ptr<GameObject>>& getGameObjects() const;
    const std::shared_ptr<GameObject>&              getGameObjectById(const std::string& objectId) const;
    std::shared_ptr<GameObject>&              getNonConstGameObjectById(const std::string& objectId);

    void                                            draw(sf::RenderWindow& window);
    void                                            update(float deltaTime);

    void deleteById(const std::string& id);

private:
    GameObjectManager()                                    = default;
    ~GameObjectManager()                                   = default;
    GameObjectManager(const GameObjectManager&)            = delete;
    GameObjectManager& operator=(const GameObjectManager&) = delete;

    std::vector<std::shared_ptr<GameObject>> gameObjects;
};
