#pragma once
#include "IRenderComponent.h"

class TileLayerComponent : public IRenderComponent
{
public:
    TileLayerComponent(const GameObject& parent);

    void addSprite(std::shared_ptr<sf::Sprite> sprite);
    void draw(sf::RenderWindow& renderWindow) override;

    int  getRenderOrder() const;
    void setRenderOrder(int order);

    bool isVisible() const;
    void setVisible(bool isVisible);

    bool init() override;
    void update(float deltaTime) override;


private:
    std::vector<std::shared_ptr<sf::Sprite>> sprites;
    int                                      renderOrder;
    bool                                     visible;
};
