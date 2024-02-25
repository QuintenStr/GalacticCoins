#pragma once
#include "IRenderComponent.h"
class TextComponent : public IRenderComponent
{
public:
    TextComponent(const GameObject& parent);

    void                      setText(std::shared_ptr<sf::Text> textToSet);
    std::shared_ptr<sf::Text> getText() const;

    void draw(sf::RenderWindow& renderWindow) override;

    int  getRenderOrder() const;
    void setRenderOrder(int order);

    bool isVisible() const;
    void setVisible(bool isVisible);

    bool init() override;
    void update(float deltaTime) override;

private:
    std::shared_ptr<sf::Text> text;
    int                       renderOrder;
    bool                      visible;
};