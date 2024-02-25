#include "stdafx.h"

#include "HumanController.h"
#include "RigidBodyComponent.h"
#include "InputManager.h"
#include "CharacterComponent.h"
#include "AssetManager.h"

HumanController::HumanController(GameObject& owner) : IController(owner)
{
}

void HumanController::update(float deltaTime)
{
    auto            length2           = [](const sf::Vector2f& vec) -> float { return vec.x * vec.x + vec.y * vec.y; };
    constexpr float acc               = 400.F;
    sf::Vector2f    accVec            = {0.F, 0.F};
    constexpr float speedLimit        = 75.F;
    constexpr float squaredSpeedLimit = speedLimit * speedLimit;

    bool isMovingUp    = InputManager::getInstance().isKeyDown("move_up", 1);
    bool isMovingDown  = InputManager::getInstance().isKeyDown("move_down", 1);
    bool isMovingLeft  = InputManager::getInstance().isKeyDown("move_left", 1);
    bool isMovingRight = InputManager::getInstance().isKeyDown("move_right", 1);
    bool isShielding = InputManager::getInstance().isKeyDown("shield", 1);

    if (isMovingLeft)
    {
        accVec.x -= acc;
    }
    if (isMovingRight)
    {
        accVec.x += acc;
    }
    if (isMovingUp)
    {
        accVec.y -= acc;
    }
    if (isMovingDown)
    {
        accVec.y += acc;
    }

    auto parent     = owner;
    auto components = parent.getComponents();

    RigidBodyComponent* bodyComp{};

    for (auto component : components)
    {
        if (auto rigidbodyComponent = dynamic_cast<RigidBodyComponent*>(component.get()))
        {
            bodyComp = rigidbodyComponent;
            break;
        }
    }

    if (bodyComp)
    {

        if (!isMovingLeft && !isMovingRight && !isMovingUp && !isMovingDown)
        {
            const float decayFactor = 0.2f;
            accVec.x -= decayFactor * bodyComp->m_velocity.x;
            accVec.y -= decayFactor * bodyComp->m_velocity.y;
        }

        sf::Vector2f newVelocity = bodyComp->m_velocity + accVec * deltaTime;

        if ((bodyComp->m_velocity.x > 0 && newVelocity.x < 0) || (bodyComp->m_velocity.x < 0 && newVelocity.x > 0))
        {
            bodyComp->m_velocity.x = 0;
        }
        else
        {
            bodyComp->m_velocity.x = newVelocity.x;
        }

        if ((bodyComp->m_velocity.y > 0 && newVelocity.y < 0) || (bodyComp->m_velocity.y < 0 && newVelocity.y > 0))
        {
            bodyComp->m_velocity.y = 0;
        }
        else
        {
            bodyComp->m_velocity.y = newVelocity.y;
        }

        if (length2(bodyComp->m_velocity) < squaredSpeedLimit)
        {
            bodyComp->m_impulses.push_back(accVec);
        }
    }

    // shielding
    if (!m_shieldActive)
    {
        m_shieldTimer += deltaTime;
    }

    // Check for shield activation
    if (isShielding && m_shieldTimer >= 5.0f)
    {
        m_shieldActive      = true;
        m_activeShieldTimer = 0.0f;
        m_shieldTimer       = 0.0f;
        activateShield();
    }
    if (isShielding && m_shieldTimer < 5.0f)
    {
        std::cout << "shield not ready yet" << std::endl;
    }

    // Update active shield timer
    if (m_shieldActive)
    {
        m_activeShieldTimer += deltaTime;
        if (m_activeShieldTimer >= 2.0f)
        {
            m_shieldActive = false;
            deactivateShield();
        }
    }
}

void HumanController::activateShield()
{
    std::cout << "Activate shield" << std::endl;

    auto&  parent     = owner;
    auto& components = parent.getComponents();

    for (auto it = components.begin(); it != components.end(); ++it)
    {
        if (*it == m_originalSprite)
        {
            parent.removeComponentsOfType<AnimatedSpriteComponent>();
            parent.addComponent(m_shieldSprite);
            break;
        }
    }

    auto& charComp = parent.getComponentByType<CharacterComponent>();
    charComp->setIsShielded(true);

    
    if (AssetManager::Sounds.find("ShieldActive") != AssetManager::Sounds.end())
    {
        std::shared_ptr<sf::Sound> sound = AssetManager::Sounds["ShieldActive"];
        sound->play();
    }
}

void HumanController::deactivateShield()
{
    std::cout << "Deactivate shield" << std::endl;

    auto& parent     = owner;
    auto& components = parent.getComponents();

    for (auto it = components.begin(); it != components.end(); ++it)
    {
        if (*it == m_shieldSprite)
        {
            parent.removeComponentsOfType<AnimatedSpriteComponent>();
            parent.addComponent(m_originalSprite);
            break;
        }
    }

    auto& charComp = parent.getComponentByType<CharacterComponent>();
    charComp->setIsShielded(false);

    if (AssetManager::Sounds.find("ShieldActive") != AssetManager::Sounds.end())
    {
        std::shared_ptr<sf::Sound> sound = AssetManager::Sounds["ShieldActive"];
        sound->stop();
    }
}

void HumanController::setOriginalSprite(std::shared_ptr<AnimatedSpriteComponent> originalSprite)
{
    m_originalSprite = originalSprite;
}

void HumanController::setShieldSprite(std::shared_ptr<AnimatedSpriteComponent> shieldSprite)
{
    m_shieldSprite = shieldSprite;
}

float HumanController::getShieldTimer()
{
    return m_shieldTimer;
}
