#pragma once
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/System/Vector2.hpp>
#include <memory>
#include <string>
#include <vector>

namespace sf
{
class Drawable;
class RenderWindow;
class Color;
} // namespace sf

namespace mmt_gd
{
/**
 * \brief Simple debug draw functionality.
 * Use the relevant draw* method to render debug stuff in the current frame
 */
class DebugDraw
{
public:
    static DebugDraw& getInstance()
    {
        static DebugDraw instance;
        return instance;
    }

    void drawLine(const sf::Vector2f& p1, const sf::Vector2f& p2, const sf::Color& color, float thickness = 1.0F);
    void drawArrow(const sf::Vector2f& origin, const sf::Vector2f& direction, const sf::Color& color, float thickness = 1.0F);
    void drawCircle(const sf::Vector2f& center,
                    float               radius,
                    const sf::Color&    outlineColor,
                    const sf::Color&    fillColor = sf::Color::Transparent);
    void drawText(const std::string& str, const sf::Vector2f& center, const sf::Color& color);
    void drawRectangle(const sf::Vector2f& position,
                       const sf::Vector2f& size,
                       const sf::Color&    outlineColor,
                       const sf::Color&    fillColor = sf::Color::Transparent);


    bool isEnabled() const
    {
        return m_enabled;
    }

    void setEnabled(const bool enabled)
    {
        m_enabled = enabled;
    }

    void update(float deltaTime);

    void draw(sf::RenderWindow& renderWindow);

    DebugDraw(const DebugDraw&)       = delete;
    DebugDraw(const DebugDraw&&)      = delete;
    void operator=(const DebugDraw&)  = delete;
    void operator=(const DebugDraw&&) = delete;

private:
    DebugDraw();
    ~DebugDraw() = default;

    bool                                       m_enabled{};
    std::vector<std::shared_ptr<sf::Drawable>> m_shapes;
    sf::Font                                   m_font;
};
} // namespace mmt_gd
