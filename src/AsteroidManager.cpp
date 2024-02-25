#include "stdafx.h"
#include "AsteroidManager.h"
#include "AssetManager.h"
#include <random>
#include <utility>
#include "GameObject.h"
#include "RigidBodyComponent.h"
#include "BoxCollidsionComponent.h"
#include "SpriteComponent.h"
#include "GameObjectManager.h"
#include "AnimatedSpriteComponent.h"

AsteroidManager::AsteroidManager() : m_asteroidCounter(0)
{
}

sf::Vector2f generateTargetVector(const sf::Vector2f& asteroidPos, const sf::Vector2f& playerPos, const sf::Vector2f& screenMiddle)
{
    std::random_device                    rd;
    std::mt19937                          gen(rd());
    std::uniform_real_distribution<float> dis(0.f, 1.f);

    sf::Vector2f direction = playerPos - screenMiddle;

    float factor = dis(gen);

    sf::Vector2f targetPos = screenMiddle + direction * factor;

    sf::Vector2f targetVector = targetPos - asteroidPos;

    return targetVector;
}

std::pair<int, int> generateAsteroidPosition(int windowWidth, int windowHeight, int offset)
{
    std::random_device              rd;
    std::mt19937                    gen(rd());
    std::uniform_int_distribution<> sideDist(0, 3); // 0: top, 1: left, 2: bottom, 3: right
    std::uniform_int_distribution<> posDist;

    int side = sideDist(gen);
    int x, y;

    switch (side)
    {
        case 0: // Top
            posDist = std::uniform_int_distribution<>(0, windowWidth);
            x       = posDist(gen);
            y       = -offset;
            break;
        case 1: // Left
            posDist = std::uniform_int_distribution<>(0, windowHeight);
            x       = -offset;
            y       = posDist(gen);
            break;
        case 2: // Bottom
            posDist = std::uniform_int_distribution<>(0, windowWidth);
            x       = posDist(gen);
            y       = windowHeight;
            break;
        case 3: // Right
            posDist = std::uniform_int_distribution<>(0, windowHeight);
            x       = windowWidth;
            y       = posDist(gen);
            break;
    }

    return std::make_pair(x, y);
}


AsteroidManager& AsteroidManager::getInstance()
{
    static AsteroidManager instance;
    return instance;
}

void AsteroidManager::init()
{
    m_asteroidCounter = 0;
    AssetManager::LoadTexture("AsteroidTexture", "../assets/stonesanimated.png");
    AssetManager::LoadSound("AsteroidHit", "../assets/crash.wav");
    AssetManager::LoadSound("AsteroidHitShield", "../assets/shieldhit.wav");
    AssetManager::LoadSound("ShieldActive", "../assets/shieldactive.wav");
}

void AsteroidManager::update(float deltaTime)
{
    // what this does is: update step to try and spawn asteroid. it starts at 5 seconds
    // if 10 sec passed, we add + 1 to dynamuc value. this then is used in square root 
    // to edit the treshhold value. its done in sqrt so it starts getting harder faster in the beginning,
    // but the longer the game runs the slower the dificulty increases
    m_timeAccumulator += deltaTime;
    m_totalRunTime += deltaTime;
    float currentThreshold = m_baseThreshold - sqrt(m_dynamicValue);

    currentThreshold = std::max(currentThreshold, 0.33f);

    if (m_timeAccumulator >= currentThreshold)
    {
        makeAsteroid();
        m_timeAccumulator = 0.0f;
    }
    if (m_totalRunTime >= 10.f)
    {
        if (m_dynamicValue < 23)
        {
            m_dynamicValue += 1;
        }
        m_totalRunTime = 0.0f;
    }
}

void AsteroidManager::deleteAsteroid(const std::string& id)
{
    GameObjectManager::getInstance().deleteById(id);
}

void AsteroidManager::makeAsteroid()
{
    m_asteroidCounter++;
    const std::string& newId = "Asteroid" + std::to_string(m_asteroidCounter);
    const auto&        newAsteroidGameObj = std::make_shared<GameObject>(newId, "Asteroid");

    auto [x, y] = generateAsteroidPosition(960, 640, 32);
    sf::Vector2f position(x, y);

    newAsteroidGameObj->setPosition(position);

    const auto& rigidBodyComp = std::make_shared<RigidBodyComponent>(*newAsteroidGameObj);

    rigidBodyComp->m_mass         = std::numeric_limits<float>::infinity();
    rigidBodyComp->m_invMass      = 0.f;
    rigidBodyComp->m_restitution  = 0.f;
    rigidBodyComp->m_acceleration = {0, 0};

    // get player gameobject by id + position;
    const auto& player        = GameObjectManager::getInstance().getGameObjectById("player1");

    auto        playerposition = player->getPosition();
    auto        objectposition = position;
    sf::Vector2f        screencenter   = {480, 320};

    sf::Vector2f desiredvelocity = generateTargetVector(objectposition, playerposition, screencenter);

    rigidBodyComp->m_velocity = desiredvelocity;

    newAsteroidGameObj->addComponent(rigidBodyComp);

    // boxcollisioncomponent
    const auto& boxColissionComp = std::make_shared<BoxCollisionComponent>(*newAsteroidGameObj, *rigidBodyComp);
    boxColissionComp->m_shape    = {static_cast<float>(newAsteroidGameObj->getPosition().x),
                                    static_cast<float>(newAsteroidGameObj->getPosition().y),
                                    static_cast<float>(32),
                                    static_cast<float>(32)};

    newAsteroidGameObj->addComponent(boxColissionComp);

    // animated sprite component
    const auto& spriteComp = std::make_shared<AnimatedSpriteComponent>(*newAsteroidGameObj);
    spriteComp->setTexture(AssetManager::Textures["AsteroidTexture"]);

    spriteComp->setAnimationDetails(4, 32, 32, 0.15f);

    newAsteroidGameObj->addComponent(spriteComp);

    GameObjectManager::getInstance().addGameObject(newAsteroidGameObj);
}
