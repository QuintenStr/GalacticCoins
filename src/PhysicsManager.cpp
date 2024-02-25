#include "stdafx.h";

#include "PhysicsManager.h";

#include "BoxCollidsionComponent.h"
#include "CharacterComponent.h";
#include "GameObjectManager.h";
#include "CoinManager.h"
#include "AsteroidManager.h"
#include "AssetManager.h"
#include "HumanController.h"
#include "MovementComponent.h"
#include "TextComponent.h"

using namespace sf;
using namespace std;

void PhysicsManager::update(float deltaTime)
{
    const float step = 1.0f / 100.0f;
    m_accumulator += deltaTime;
    while (m_accumulator > step)
    {
        updateStep(step);
        m_accumulator -= step;
    }
}

void PhysicsManager::updateStep(float delta)
{
    m_manifolds.clear();

    getBodies();
    findCollisions();
    resolveCollisions();
}

static bool aabbVsAabb(const FloatRect& a, const FloatRect& b, Vector2f& normal, float& penetration)
{
    auto getCenter = [](const FloatRect& rect) -> Vector2f
    { return Vector2f(rect.left, rect.top) + 0.5F * Vector2f(rect.width, rect.height); };

    const Vector2f n        = getCenter(b) - getCenter(a);
    const float    aExtentX = a.width * 0.5F;
    const float    bExtentX = b.width * 0.5F;
    const float    xOverlap = aExtentX + bExtentX - abs(n.x);

    if (xOverlap > 0)
    {
        const float aExtentY = a.height * 0.5F;
        const float bExtentY = b.height * 0.5F;
        const float yOverlap = aExtentY + bExtentY - abs(n.y);

        if (yOverlap > 0)
        {
            if (xOverlap < yOverlap)
            {
                if (n.x < 0)
                {
                    normal = Vector2f(1.0F, 0.0F);
                }
                else
                {
                    normal = Vector2f(-1.0F, 0.0F);
                }
                penetration = xOverlap;
                return true;
            }
            if (n.y < 0)
            {
                normal = Vector2f(0, 1);
            }
            else
            {
                normal = Vector2f(0, -1);
            }
            penetration = yOverlap;
            return true;
        }
    }
    return false;
}

void PhysicsManager::getBodies()
{
    m_bodies.clear();
    for (auto item : GameObjectManager::getInstance().getGameObjects())
    {
        for (auto component : item->getComponents())
        {
            RigidBodyComponent* rigidBodyComponent = dynamic_cast<RigidBodyComponent*>(component.get());

            if (rigidBodyComponent != nullptr)
            {
                m_bodies.push_back(rigidBodyComponent);
            }
        }
    }
}


void PhysicsManager::findCollisions()
{
    for (auto itA = m_bodies.begin(); itA != m_bodies.end(); ++itA)
    {
        auto& body1 = *itA;
        for (auto itB = itA; itB != m_bodies.end(); ++itB)
        {
            if (itA == itB)
            {
                continue;
            }

            auto& body2 = *itB;

            if (body1->m_mass == 0 && body2->m_mass == 0)
            {
                continue;
            }

            auto& parent1 = const_cast<GameObject&>(body1->getParent());
            auto& parent2 = const_cast<GameObject&>(body2->getParent());

            std::shared_ptr<BoxCollisionComponent> box1 = parent1.getComponentByType<BoxCollisionComponent>();
            std::shared_ptr<BoxCollisionComponent> box2 = parent2.getComponentByType<BoxCollisionComponent>();


            if (box1 != nullptr && box2 != nullptr)
            {
                FloatRect body1shape = FloatRect(parent1.getPosition().x,
                                                 parent1.getPosition().y,
                                                 box1->m_shape.width,
                                                 box1->m_shape.height);

                FloatRect body2shape = FloatRect(parent2.getPosition().x,
                                                 parent2.getPosition().y,
                                                 box2->m_shape.width,
                                                 box2->m_shape.height);

                Vector2f normal;
                float    penetration = NAN;

                if (aabbVsAabb(body1shape, body2shape, normal, penetration))
                {
                    Manifold manifold;
                    manifold.m_body1       = body1;
                    manifold.m_body2       = body2;
                    manifold.m_normal      = normal;
                    manifold.m_penetration = penetration;

                    m_manifolds.push_back(manifold);
                }
            }
        }
    }
}

void PhysicsManager::substepResolvecollision(Manifold man)
{
    if (man.m_body1->m_invMass == 0 && man.m_body2->m_invMass == 0)
    {
        return;
    }

    sf::Vector2f rv = man.m_body1->m_velocity - man.m_body2->m_velocity;

    float velAlongNormal = rv.x * man.m_normal.x + rv.y * man.m_normal.y;

    if (velAlongNormal > 0)
        return;

    float e = min(man.m_body1->m_restitution, man.m_body2->m_restitution);

    const float    j       = -(1.0f + e) * velAlongNormal / (man.m_body1->m_invMass + man.m_body2->m_invMass);
    const Vector2f impulse = j * man.m_normal;

    man.m_body1->m_velocity += man.m_body1->m_invMass * impulse;
    man.m_body2->m_velocity -= man.m_body2->m_invMass * impulse;

    positionalCorrection(man);
}

void PhysicsManager::resolveCollisions()
{
    for (auto man : m_manifolds)
    {
        auto& parent1 = const_cast<GameObject&>(man.m_body1->getParent());
        auto& parent2 = const_cast<GameObject&>(man.m_body2->getParent());

        // player into wall or wall into player
        if (parent1.getType() == "Character" && parent2.getType() == "Obstacle" || parent1.getType() == "Obstacle" && parent2.getType() == "Character")
        {
            substepResolvecollision(man);
        }

        // player into coin or coin into player
        if (parent1.getType() == "Character" && parent2.getType() == "Coin" || parent1.getType() == "Coin" && parent2.getType() == "Character")
        {
            if (parent1.getType() == "Character" && parent2.getType() == "Coin")
            {
                if (AssetManager::Sounds.find("CoinPickup") != AssetManager::Sounds.end())
                {
                    std::shared_ptr<sf::Sound> sound = AssetManager::Sounds["CoinPickup"];
                    sound->play();
                }

                const auto& characterComp = parent1.getComponentByType<CharacterComponent>();
                characterComp->PickupCoin();
                CoinManager::getInstance().deleteCoinAndMakeNewOne(parent2.getId());

                auto& coinCounterGameObj = GameObjectManager::getInstance().getNonConstGameObjectById("CoinCounter");
                if (coinCounterGameObj)
                {
                    auto& coincounterComp = coinCounterGameObj->getComponentByType<TextComponent>();
                    if (coincounterComp)
                    {
                        auto coincounterText = coincounterComp->getText();
                        coincounterText->setString("Coins: " + std::to_string(characterComp->getCoinCount()));
                    }
                }
                continue;
            }
            if (parent1.getType() == "Coin" && parent2.getType() == "Character")
            {
                if (AssetManager::Sounds.find("CoinPickup") != AssetManager::Sounds.end())
                {
                    std::shared_ptr<sf::Sound> sound = AssetManager::Sounds["CoinPickup"];
                    sound->play();
                }

                const auto& characterComp = parent2.getComponentByType<CharacterComponent>();
                characterComp->PickupCoin();
                CoinManager::getInstance().deleteCoinAndMakeNewOne(parent1.getId());

                auto& coinCounterGameObj = GameObjectManager::getInstance().getNonConstGameObjectById("CoinCounter");
                if (coinCounterGameObj)
                {
                    auto& coincounterComp = coinCounterGameObj->getComponentByType<TextComponent>();
                    if (coincounterComp)
                    {
                        auto coincounterText = coincounterComp->getText();
                        coincounterText->setString("Coins: " + std::to_string(characterComp->getCoinCount()));
                    }
                }
                continue;
            }
        }

        
        // player into asteroid or asteroid into player
        if (parent1.getType() == "Character" && parent2.getType() == "Asteroid" || parent1.getType() == "Asteroid" && parent2.getType() == "Character")
        {
            // shield not active
            bool shieldActive;
            if (parent1.getType() == "Character")
            {
                auto& charComp = parent1.getComponentByType<CharacterComponent>();
                shieldActive   = charComp->getIsShielded();
            }
            if (parent2.getType() == "Character")
            {
                auto& charComp = parent2.getComponentByType<CharacterComponent>();
                shieldActive   = charComp->getIsShielded();
            }

            if (shieldActive == false)
            {
                if (parent1.getType() == "Character" && parent2.getType() == "Asteroid")
                {
                    if (AssetManager::Sounds.find("AsteroidHit") != AssetManager::Sounds.end())
                    {
                        std::shared_ptr<sf::Sound> sound = AssetManager::Sounds["AsteroidHit"];
                        sound->play();
                    }

                    const auto& characterComp = parent1.getComponentByType<CharacterComponent>();
                    characterComp->TakeDamage();

                    substepResolvecollision(man);
                    AsteroidManager::getInstance().deleteAsteroid(parent2.getId());

                    continue;
                }
                if (parent1.getType() == "Asteroid" && parent2.getType() == "Character")
                {
                    if (AssetManager::Sounds.find("AsteroidHit") != AssetManager::Sounds.end())
                    {
                        std::shared_ptr<sf::Sound> sound = AssetManager::Sounds["AsteroidHit"];
                        sound->play();
                    }

                    const auto& characterComp = parent2.getComponentByType<CharacterComponent>();
                    characterComp->TakeDamage();

                    substepResolvecollision(man);
                    AsteroidManager::getInstance().deleteAsteroid(parent1.getId());

                    continue;
                }

            }
            if (shieldActive == true)
            {
                if (parent1.getType() == "Character" && parent2.getType() == "Asteroid")
                {
                    AsteroidManager::getInstance().deleteAsteroid(parent2.getId());

                    if (AssetManager::Sounds.find("AsteroidHitShield") != AssetManager::Sounds.end())
                    {
                        std::shared_ptr<sf::Sound> sound = AssetManager::Sounds["AsteroidHitShield"];
                        sound->play();
                    }

                    auto& movementComp = parent1.getComponentByType<MovementComponent>();
                    auto& controllerComp = movementComp->getController();

                    std::shared_ptr<HumanController> controllerCompHuman = std::dynamic_pointer_cast<HumanController>(controllerComp);
                    controllerCompHuman->deactivateShield();

                    continue;
                }
                if (parent1.getType() == "Asteroid" && parent2.getType() == "Character")
                {
                    AsteroidManager::getInstance().deleteAsteroid(parent1.getId());

                    if (AssetManager::Sounds.find("AsteroidHitShield") != AssetManager::Sounds.end())
                    {
                        std::shared_ptr<sf::Sound> sound = AssetManager::Sounds["AsteroidHitShield"];
                        sound->play();
                    }

                    auto& movementComp   = parent2.getComponentByType<MovementComponent>();
                    auto& controllerComp = movementComp->getController();

                    std::shared_ptr<HumanController> controllerCompHuman = std::dynamic_pointer_cast<HumanController>(controllerComp);
                    controllerCompHuman->deactivateShield();


                    continue;
                }
            }
        }
    }
}

void PhysicsManager::positionalCorrection(Manifold man)
{
    const float percent = 0.2f;
    const float slop    = 0.01f;

    Vector2f correction = std::max(man.m_penetration - slop, 0.0f) / (man.m_body1->m_invMass + man.m_body2->m_invMass) *
                          percent * man.m_normal;

    auto& parent1 = const_cast<GameObject&>(man.m_body1->getParent());
    parent1.move(man.m_body1->m_invMass * correction);

    auto& parent2 = const_cast<GameObject&>(man.m_body2->getParent());
    parent2.move(-man.m_body2->m_invMass * correction);
}

PhysicsManager::PhysicsManager()
{
}
