#include "stdafx.h"
#include "CoinManager.h"

#include "AnimatedSpriteComponent.h"
#include "BoxCollidsionComponent.h"
#include "RigidBodyComponent.h"

#include <limits>
#include <random>

CoinManager::CoinManager() : m_coinCounter(0)
{
}

CoinManager& CoinManager::getInstance()
{
    static CoinManager instance;
    return instance;
}

void CoinManager::init()
{
    m_coinCounter = 0;
    AssetManager::LoadTexture("CoinTexture", "../assets/coins.png");
    AssetManager::LoadSound("CoinPickup", "../assets/pickupCoin.wav");

    for (int n = 0; n < 3; n++)
    {
        makeCoin();
    }
}

void CoinManager::deleteCoinAndMakeNewOne(const std::string& id)
{
    auto coin = GameObjectManager::getInstance().getGameObjectById(id);
    if (coin)
    {
        int x = coin->getPosition().x;
        int y = coin->getPosition().y;
        for (int dx = 0; dx < 32; ++dx)
        {
            for (int dy = 0; dy < 32; ++dy)
            {
                m_coinPositions.erase({x + dx, y + dy});
            }
        }
    }
    GameObjectManager::getInstance().deleteById(id);
    makeCoin();
}

std::pair<int, int> CoinManager::generateRandomCoordinates()
{
    std::random_device              rd;
    std::mt19937                    gen(rd());
    std::uniform_int_distribution<> dis_x(0, 960 - 32);
    std::uniform_int_distribution<> dis_y(0, 640 - 32);

    int x, y;
    do
    {
        x = dis_x(gen);
        y = dis_y(gen);
    } while (isOverlapping(x, y));

    return {x, y};
}

bool CoinManager::isOverlapping(int x, int y)
{
    for (int dx = 0; dx < 32; ++dx)
    {
        for (int dy = 0; dy < 32; ++dy)
        {
            if (m_coinPositions.find({x + dx, y + dy}) != m_coinPositions.end())
            {
                return true;
            }
        }
    }
    return false;
}

void CoinManager::makeCoin()
{
    m_coinCounter++;
    const std::string& newId          = "Coin" + std::to_string(m_coinCounter);
    const auto&        newCoinGameObj = std::make_shared<GameObject>(newId, "Coin");

    auto [x, y] = generateRandomCoordinates();
    for (int dx = 0; dx < 32; ++dx)
    {
        for (int dy = 0; dy < 32; ++dy)
        {
            m_coinPositions.insert({x + dx, y + dy});
        }
    }
    sf::Vector2f position(x, y);
    newCoinGameObj->setPosition(position);

    // rigidbodycomponent
    const auto& rigidBodyComp = std::make_shared<RigidBodyComponent>(*newCoinGameObj);

    rigidBodyComp->m_mass         = std::numeric_limits<float>::infinity();
    rigidBodyComp->m_invMass      = 0.f;
    rigidBodyComp->m_restitution  = 0.f;
    rigidBodyComp->m_acceleration = {0, 0};
    rigidBodyComp->m_velocity     = {0, 0};

    newCoinGameObj->addComponent(rigidBodyComp);

    // boxcollisioncomponent
    const auto& boxColissionComp = std::make_shared<BoxCollisionComponent>(*newCoinGameObj, *rigidBodyComp);
    boxColissionComp->m_shape    = {static_cast<float>(newCoinGameObj->getPosition().x),
                                    static_cast<float>(newCoinGameObj->getPosition().y),
                                    static_cast<float>(32),
                                    static_cast<float>(32)};

    newCoinGameObj->addComponent(boxColissionComp);

    // animated sprite component
    const auto& spriteComp = std::make_shared<AnimatedSpriteComponent>(*newCoinGameObj);
    spriteComp->setTexture(AssetManager::Textures["CoinTexture"]);

    spriteComp->setAnimationDetails(9, 32, 32, 0.111111111f);
    spriteComp->initialPositionUpdate();

    newCoinGameObj->addComponent(spriteComp);

    GameObjectManager::getInstance().addGameObject(newCoinGameObj);
}
