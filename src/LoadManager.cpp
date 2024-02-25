#include "stdafx.h"

#include "LoadManager.h"

#include "BoxCollidsionComponent.h"
#include "CharacterComponent.h"
#include "HumanController.h"
#include "MovementComponent.h"
#include "RigidBodyComponent.h"
#include "SpriteComponent.h"
#include "TileLayerComponent.h"
#include "GameObject.h"
#include "GameObjectManager.h"
#include "AssetManager.h"
#include "AnimatedSpriteComponent.h"
#include "CoinManager.h"
#include "AsteroidManager.h"
#include "HPBarSpriteComponent.h"
#include "ShieldBarSpriteComponent.h"
#include "TextComponent.h"


using namespace sf;
using namespace std;

struct GameObjectInfo
{
    std::string id;
    std::string type;
};

LoadManager& LoadManager::getInstance()
{
    static LoadManager instance;
    return instance;
}

std::shared_ptr<HumanController> humanControllerPointer = nullptr;

void LoadManager::loadMap(const fs::path& resourcePath, const fs::path& filename, const sf::Vector2f& offset)
{
    m_resourcePath = resourcePath;

    err() << "Started loading map" << std::endl;

    tson::Tileson                    t;
    const std::unique_ptr<tson::Map> map = t.parse(resourcePath / filename);

    if (map->getStatus() == tson::ParseStatus::OK)
    {
        err() << "Successfully parsed input file" << std::endl;
        err() << "Load tile map with size: " << map->getSize().x << ", " << map->getSize().y
              << " and tile size: " << map->getTileSize().x << ", " << map->getTileSize().y << std::endl;

        for (auto& tileSet : map->getTilesets())
        {
            fs::path tileSetPath = tileSet.getImage().u8string();
            AssetManager::LoadTileSetTexture(tileSet.getName(), (m_resourcePath / tileSetPath).string());
        }
    }
    else
    {
        err() << "Parse error: " << map->getStatusMessage() << std::endl;
    }

    std::map<std::string, GameObjectInfo> uniqueGameObjectInfo;

    // get all gameobject ids
    for (auto group : map->getLayers())
    {
        if (group.getType() == tson::LayerType::ObjectGroup)
        {
            for (auto object : group.getObjects())
            {
                GameObjectInfo info;
                info.type = object.getType();

                for (const auto& property : object.getProperties().getProperties())
                {
                    if (property.second.getName() == "GameObjectId" && property.second.getType() == tson::Type::String)
                    {
                        info.id = property.second.getValue<std::string>();
                        // Insert only if the id is unique
                        if (uniqueGameObjectInfo.find(info.id) == uniqueGameObjectInfo.end())
                        {
                            uniqueGameObjectInfo[info.id] = info;
                        }
                    }
                }
            }
        }
    }

    // foreach gameobject if not exist make
    for (const auto& entry : uniqueGameObjectInfo)
    {
        const auto& gameObjectId   = entry.second.id;
        auto        gameObjectType = entry.second.type;

        // if gameobject with gameobjectid is nullptr make new gameobject and add to GameObjectManager
        err() << "Looking for GameObject with ID: " << gameObjectId << "\n";
        if (GameObjectManager::getInstance().getGameObjectById(gameObjectId) == nullptr)
        {
            err() << "No GameObject with ID: " << gameObjectId << " was found. \n";
            if (gameObjectType == "Sprite")
            {
                gameObjectType = "Character";
            }
            if (gameObjectType == "AnimatedSpritePlayer")
            {
                gameObjectType = "Character";
            }
            if (gameObjectType == "AABB")
            {
                gameObjectType = "Obstacle";
            }
            if (gameObjectId == "Map")
            {
                gameObjectType = "Map";
            }
            const auto& newGameObj = std::make_shared<GameObject>(gameObjectId, gameObjectType);
            err() << "Created GameObject with ID: " << gameObjectId << "\n";
            GameObjectManager::getInstance().addGameObject(newGameObj);
            err() << "Added GameObject to GameObjectManager with ID: " << gameObjectId << "\n";
        }
        else
        {
            err() << "GameObject with ID: " << gameObjectId
                  << " was found. Skipping creation and adding to GameObjectManager. \n";
        }
    }

    // tile layer part
    const auto& mapGameObj = GameObjectManager::getInstance().getGameObjectById("Map");
    for (int layerIdx = 0; layerIdx < static_cast<int>(map->getLayers().size()); layerIdx++)
    {
        auto layer = map->getLayers()[layerIdx];
        if (layer.getType() == tson::LayerType::TileLayer)
        {
            err() << "Load layer: " << layer.getName() << " with width: " << layer.getSize().x
                  << " and height: " << layer.getSize().y << std::endl;

            const int size = layer.getSize().x * layer.getSize().y;


            const auto& tileLayerComp = std::make_shared<TileLayerComponent>(*mapGameObj);
            mapGameObj->addComponent(tileLayerComp);

            for (const auto& property : layer.getProperties().getProperties())
            {
                if (property.second.getName() == "renderorder" && property.second.getType() == tson::Type::Int)
                {
                    int renderOrder = property.second.getValue<int>();
                    tileLayerComp->setRenderOrder(renderOrder);
                }
            }

            for (int i = 0; i < size; i++)
            {
                const auto gid = layer.getData()[i];

                if (gid == 0)
                {
                    continue;
                }

                const auto* const tileSet = map->getTilesetByGid(gid);
                const Vector2i    tileSize(map->getTileSize().x, map->getTileSize().y);
                Texture&          texture = *AssetManager::m_tileSetTexture[tileSet->getName()];

                Vector2f position;
                position.x = i % layer.getSize().x * static_cast<float>(tileSize.x);
                position.y = i / layer.getSize().x * static_cast<float>(tileSize.y);
                position += offset;

                const int tileCountX = texture.getSize().x / tileSize.x;

                const int idx  = gid - tileSet->getFirstgid();
                const int idxX = idx % tileCountX;
                const int idxY = idx / tileCountX;

                IntRect source(idxX * tileSize.x, idxY * tileSize.y, tileSize.x, tileSize.y);

                auto sprite = make_shared<Sprite>();
                sprite->setTexture(texture);
                sprite->setTextureRect(source);
                sprite->setPosition(position.x, position.y);

                tileLayerComp->addSprite(sprite);
            }
        }
    }
    err() << "Loaded all tiled layers" << std::endl;

    // component layer part
    for (auto group : map->getLayers())
    {
        if (group.getType() == tson::LayerType::ObjectGroup)
        {
            for (auto object : group.getObjects())
            {
                // get gameobject id from custom properties

                std::string gameObjectId;
                for (const auto& property : object.getProperties().getProperties())
                {
                    if (property.second.getName() == "GameObjectId" && property.second.getType() == tson::Type::String)
                    {
                        gameObjectId = property.second.getValue<string>();
                    }
                }

                const auto& GameObject = GameObjectManager::getInstance().getGameObjectById(gameObjectId);



                if (object.getType() == "Sprite")
                {
                    // gameobject position
                    Vector2f position(object.getPosition().x, object.getPosition().y);
                    position += offset;

                    GameObject->setPosition(position);

                    // sprite component
                    const auto& spriteComp = std::make_shared<SpriteComponent>(*GameObject);
                    for (const auto& property : object.getProperties().getProperties())
                    {
                        if (property.second.getName() == "Texture" && property.second.getType() == tson::Type::String)
                        {
                            string texturePath = property.second.getValue<string>();
                            AssetManager::LoadTexture(object.getName(), (m_resourcePath / texturePath).string());
                            err() << "Loaded texture for sprite component for game object: " << object.getName()
                                  << std::endl;
                        }
                        if (property.second.getName() == "renderorder" && property.second.getType() == tson::Type::Int)
                        {
                            int renderOrder = property.second.getValue<int>();
                            spriteComp->setRenderOrder(renderOrder);
                        }
                    }

                    Texture&    texture = *AssetManager::Textures[object.getName()];
                    const auto& sprite  = std::make_shared<Sprite>(texture);

                    spriteComp->setSprite(sprite);
                    GameObject->addComponent(spriteComp);

                    // rigidbodycomponent

                    const auto& rigidBodyComp = std::make_shared<RigidBodyComponent>(*GameObject);

                    for (const auto& property : object.getProperties().getProperties())
                    {
                        if (property.second.getName() == "m_mass" && property.second.getType() == tson::Type::Float)
                        {
                            rigidBodyComp->m_mass    = property.second.getValue<float>();
                            rigidBodyComp->m_invMass = (1 / property.second.getValue<float>());
                        }
                        if (property.second.getName() == "m_restitution" && property.second.getType() == tson::Type::Float)
                        {
                            rigidBodyComp->m_restitution = property.second.getValue<float>();
                        }
                    }
                    rigidBodyComp->m_acceleration = {0, 0};
                    rigidBodyComp->m_velocity     = {0, 0};

                    GameObject->addComponent(rigidBodyComp);

                    // boxcolissioncomponent

                    const auto& boxColissionComp = std::make_shared<BoxCollisionComponent>(*GameObject, *rigidBodyComp);
                    boxColissionComp->m_shape    = {static_cast<float>(object.getPosition().x),
                                                    static_cast<float>(object.getPosition().y),
                                                    static_cast<float>(object.getSize().x),
                                                    static_cast<float>(object.getSize().y)};

                    GameObject->addComponent(boxColissionComp);
                }

                
                if (gameObjectId == "player1")
                {
                    auto movementComponent = std::make_shared<MovementComponent>(*GameObject);
                    humanControllerPointer = std::make_shared<HumanController>(*GameObject);
                    movementComponent->setStrategy(humanControllerPointer);

                    GameObject->addComponent(movementComponent);

                    const auto& characterComponent = std::make_shared<CharacterComponent>(*GameObject);
                    GameObject->addComponent(characterComponent);
                }





                if (object.getType() == "AnimatedSpritePlayer")
                {
                    // gameobject position
                    Vector2f position(object.getPosition().x, object.getPosition().y);
                    position += offset;

                    GameObject->setPosition(position);

                    // normal sprite component
                    const auto& spriteComp = std::make_shared<AnimatedSpriteComponent>(*GameObject);
                    for (const auto& property : object.getProperties().getProperties())
                    {
                        if (property.second.getName() == "Texture" && property.second.getType() == tson::Type::String)
                        {
                            string texturePath = property.second.getValue<string>();
                            AssetManager::LoadTexture(object.getName(), (m_resourcePath / texturePath).string());
                            err() << "Loaded texture for sprite component for game object: " << object.getName()
                                  << std::endl;
                        }
                        if (property.second.getName() == "renderorder" && property.second.getType() == tson::Type::Int)
                        {
                            int renderOrder = property.second.getValue<int>();
                            spriteComp->setRenderOrder(renderOrder);
                        }
                    }

                    spriteComp->setTexture(AssetManager::Textures[object.getName()]);
                    spriteComp->setAnimationDetails(4, 32, 32, 0.2f);

                    GameObject->addComponent(spriteComp);


                    // shield sprite component
                    const auto& spriteCompShield = std::make_shared<AnimatedSpriteComponent>(*GameObject);
                    AssetManager::LoadTexture("ShieldedUfoSprite", "../assets/UFOshielded.png");
                    spriteCompShield->setRenderOrder(3);
                    spriteCompShield->setVisible(true);

                    spriteCompShield->setTexture(AssetManager::Textures["ShieldedUfoSprite"]);
                    spriteCompShield->setAnimationDetails(4, 32, 32, 0.2f);

                    humanControllerPointer->setShieldSprite(spriteCompShield);
                    humanControllerPointer->setOriginalSprite(spriteComp);

                    // rigidbodycomponent

                    const auto& rigidBodyComp = std::make_shared<RigidBodyComponent>(*GameObject);

                    for (const auto& property : object.getProperties().getProperties())
                    {
                        if (property.second.getName() == "m_mass" && property.second.getType() == tson::Type::Float)
                        {
                            rigidBodyComp->m_mass    = property.second.getValue<float>();
                            rigidBodyComp->m_invMass = (1 / property.second.getValue<float>());
                        }
                        if (property.second.getName() == "m_restitution" && property.second.getType() == tson::Type::Float)
                        {
                            rigidBodyComp->m_restitution = property.second.getValue<float>();
                        }
                    }
                    rigidBodyComp->m_acceleration = {0, 0};
                    rigidBodyComp->m_velocity     = {0, 0};

                    GameObject->addComponent(rigidBodyComp);

                    // boxcolissioncomponent

                    const auto& boxColissionComp = std::make_shared<BoxCollisionComponent>(*GameObject, *rigidBodyComp);
                    boxColissionComp->m_shape    = {static_cast<float>(object.getPosition().x),
                                                    static_cast<float>(object.getPosition().y),
                                                    static_cast<float>(object.getSize().x),
                                                    static_cast<float>(object.getSize().y)};

                    GameObject->addComponent(boxColissionComp);
                }











                // walls of map with 0 restitution 0 inv mass and infinite mass
                if (object.getType() == "AABB")
                {
                    // gameobject position
                    Vector2f position(object.getPosition().x, object.getPosition().y);
                    position += offset;

                    GameObject->setPosition(position);

                    // rigidbodycomponent
                    const auto& rigidBodyComp = std::make_shared<RigidBodyComponent>(*GameObject);

                    for (const auto& property : object.getProperties().getProperties())
                    {
                        if (property.second.getName() == "m_inversemass" && property.second.getType() == tson::Type::Float)
                        {
                            rigidBodyComp->m_mass    = std::numeric_limits<float>::infinity();
                            rigidBodyComp->m_invMass = property.second.getValue<float>();
                        }
                        if (property.second.getName() == "m_restitution" && property.second.getType() == tson::Type::Float)
                        {
                            rigidBodyComp->m_restitution = property.second.getValue<float>();
                        }
                    }
                    rigidBodyComp->m_acceleration = {0, 0};
                    rigidBodyComp->m_velocity     = {0, 0};

                    GameObject->addComponent(rigidBodyComp);

                    // boxcolissioncomponent

                    const auto& boxColissionComp = std::make_shared<BoxCollisionComponent>(*GameObject, *rigidBodyComp);
                    boxColissionComp->m_shape    = {static_cast<float>(object.getPosition().x),
                                                    static_cast<float>(object.getPosition().y),
                                                    static_cast<float>(object.getSize().x),
                                                    static_cast<float>(object.getSize().y)};

                    GameObject->addComponent(boxColissionComp);
                }

                if (object.getType() == "Obstacle")
                {
                    // gameobject position
                    Vector2f position(object.getPosition().x, object.getPosition().y);
                    position += offset;

                    GameObject->setPosition(position);

                    // rigidbodycomponent
                    const auto& rigidBodyComp = std::make_shared<RigidBodyComponent>(*GameObject);

                    for (const auto& property : object.getProperties().getProperties())
                    {
                        if (property.second.getName() == "m_inversemass" && property.second.getType() == tson::Type::Float)
                        {
                            rigidBodyComp->m_mass    = std::numeric_limits<float>::infinity();
                            rigidBodyComp->m_invMass = property.second.getValue<float>();
                        }
                        if (property.second.getName() == "m_restitution" && property.second.getType() == tson::Type::Float)
                        {
                            rigidBodyComp->m_restitution = property.second.getValue<float>();
                        }
                    }
                    rigidBodyComp->m_acceleration = {0, 0};
                    rigidBodyComp->m_velocity     = {0, 0};

                    GameObject->addComponent(rigidBodyComp);

                    // boxcolissioncomponent

                    const auto& boxColissionComp = std::make_shared<BoxCollisionComponent>(*GameObject, *rigidBodyComp);
                    boxColissionComp->m_shape    = {static_cast<float>(object.getPosition().x),
                                                    static_cast<float>(object.getPosition().y),
                                                    static_cast<float>(object.getSize().x),
                                                    static_cast<float>(object.getSize().y)};

                    GameObject->addComponent(boxColissionComp);
                }
            }
        }
    }
    // to do obstacle manager inits
    CoinManager::getInstance().init();
    AsteroidManager::getInstance().init();

    // make game object hp bar
    const auto& hpBarGameObj = std::make_shared<GameObject>("HpBar", "UI");
    Vector2f position(0, 606);
    hpBarGameObj->setPosition(position);

    // sprite component
    const auto& spriteComp = std::make_shared<HPBarSpriteComponent>(*hpBarGameObj);
    AssetManager::LoadTexture("HpBar", "../assets/pink_hp_2.png");
    spriteComp->setRenderOrder(9999);
    Texture&    texture = *AssetManager::Textures["HpBar"];
    const auto& sprite  = std::make_shared<Sprite>(texture);

    spriteComp->setSprite(sprite);
    hpBarGameObj->addComponent(spriteComp);

    GameObjectManager::getInstance().addGameObject(hpBarGameObj);

    // make game object shield bar
    const auto& shieldBarGameObj = std::make_shared<GameObject>("ShieldBar", "UI");
    Vector2f    position2(856, 606);
    shieldBarGameObj->setPosition(position2);

    // sprite component
    const auto& spriteComp2 = std::make_shared<ShieldBarSpriteComponent>(*shieldBarGameObj);
    AssetManager::LoadTexture("ShieldBar", "../assets/blue_shield_1.png");
    spriteComp2->setRenderOrder(9999);
    Texture&    texture2 = *AssetManager::Textures["ShieldBar"];
    const auto& sprite2  = std::make_shared<Sprite>(texture2);

    spriteComp2->setSprite(sprite2);
    shieldBarGameObj->addComponent(spriteComp2);

    GameObjectManager::getInstance().addGameObject(shieldBarGameObj);

    // make game object coin counter
    const auto& coinCounterGameObj = std::make_shared<GameObject>("CoinCounter", "UI");
    Vector2f    position3(1, 1);
    coinCounterGameObj->setPosition(position3);

    // text component
    const auto& coinCounterTextComp = std::make_shared<TextComponent>(*coinCounterGameObj);
    AssetManager::LoadFont("Arial", "../assets/arial.ttf");
    coinCounterTextComp->setRenderOrder(9999);

    Font& coinCounterFont = *AssetManager::Fonts["Arial"];
    Text  cointext;
    cointext.setFont(coinCounterFont);
    cointext.setCharacterSize(18);
    cointext.setFillColor(sf::Color::White);
    cointext.setString("Coins: 0");

    auto cointextPtr = std::make_shared<sf::Text>(cointext);
    coinCounterTextComp->setText(cointextPtr);
    coinCounterGameObj->addComponent(coinCounterTextComp);

    GameObjectManager::getInstance().addGameObject(coinCounterGameObj);

   // // make game object timer
   // const auto& timerGameObj = std::make_shared<GameObject>("Timer", "UI");
   // Vector2f    position4(875, 1);
   // timerGameObj->setPosition(position4);

   // // text component
   // const auto& timerTextComp = std::make_shared<TextComponent>(*timerGameObj);
   // timerTextComp->setRenderOrder(9999);

   // Font& timerFont = *AssetManager::Fonts["Arial"];
   // Text  timerText;
   // timerText.setFont(timerFont);
   // timerText.setCharacterSize(18);
   // timerText.setFillColor(sf::Color::White);
   // timerText.setString("Time: 0s");

   //auto timertextPtr = std::make_shared<sf::Text>(timerText);
   //timerTextComp->setText(timertextPtr);
   //timerGameObj->addComponent(timerTextComp);

   //GameObjectManager::getInstance().addGameObject(timerGameObj);
}


LoadManager::LoadManager()
{
}
