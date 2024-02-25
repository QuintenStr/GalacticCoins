#include "stdafx.h"

#include "GameObjectManager.h"
#include "RigidBodyComponent.h"
#include "GameStateManager.h"

void GameObjectManager::addGameObject(std::shared_ptr<GameObject> object)
{
    gameObjects.push_back(object);
}

const std::vector<std::shared_ptr<GameObject>>& GameObjectManager::getGameObjects() const
{
    return gameObjects;
}

const std::shared_ptr<GameObject>& GameObjectManager::getGameObjectById(const std::string& objectId) const
{
    for (const auto& object : gameObjects)
    {
        if (object->getId() == objectId)
        {
            return object;
        }
    }

    static std::shared_ptr<GameObject> nullObject = nullptr;
    return nullObject;
}

std::shared_ptr<GameObject>& GameObjectManager::getNonConstGameObjectById(const std::string& objectId)
{
    for (auto& object : gameObjects)
    {
        if (object->getId() == objectId)
        {
            return object;
        }
    }

    static std::shared_ptr<GameObject> nullObject = nullptr;
    return nullObject;
}


void GameObjectManager::draw(sf::RenderWindow& window)
{
    for (const auto& object : gameObjects)
    {
        object->draw(window);
    }
}

void GameObjectManager::update(float deltaTime)
{
    for (const auto& object : gameObjects)
    {
        object->update(deltaTime);
    }

    // deleting asteroids if out of bounds
    sf::Vector2f topLeft(-100, -100);
    sf::Vector2f bottomRight(1060, 740);

    // Remove Asteroids outside the specified rectangle
    auto it = std::remove_if(gameObjects.begin(),
                             gameObjects.end(),
                             [&topLeft, &bottomRight](const std::shared_ptr<GameObject>& object)
                             {
                                 auto pos             = object->getPosition();
                                 bool isOutsideBounds = pos.x < topLeft.x || pos.y < topLeft.y ||
                                                        pos.x > bottomRight.x || pos.y > bottomRight.y;
                                 bool isAsteroid = object->getType() == "Asteroid";
                                 return isAsteroid && isOutsideBounds;
                             });

    if (it != gameObjects.end())
    {
        // Erase the removed elements
        gameObjects.erase(it, gameObjects.end());
    }
}

void GameObjectManager::deleteById(const std::string& id)
{
    // real full delete gives errors so soft delete -> moving outside of playable area
    auto it = std::remove_if(gameObjects.begin(),
                             gameObjects.end(),
                             [&id](const std::shared_ptr<GameObject>& object) { return object->getId() == id; });

    if (it != gameObjects.end())
    {
        gameObjects.erase(it);
    }
}