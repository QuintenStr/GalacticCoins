#include "stdafx.h"
#include "TextComponent.h"
#include "GameObject.h"

TextComponent::TextComponent(const GameObject& parent) : IRenderComponent(parent)
{
}
bool TextComponent::init()
{
    return true;
}

void TextComponent::update(float deltaTime)
{
}

void TextComponent::draw(sf::RenderWindow& renderWindow)
{
    if (text)
    {
        text->setPosition(this->getParent().getPosition()); // Assuming m_parent has the correct position
        renderWindow.draw(*text);
    }
}


void TextComponent::setText(std::shared_ptr<sf::Text> textToSet)
{
    this->text = textToSet;
}

int TextComponent::getRenderOrder() const
{
    return renderOrder;
}

std::shared_ptr<sf::Text> TextComponent::getText() const
{
    return text;
}

void TextComponent::setRenderOrder(int order)
{
    renderOrder = order;
}

bool TextComponent::isVisible() const
{
    return visible;
}

void TextComponent::setVisible(bool isVisible)
{
    visible = isVisible;
}