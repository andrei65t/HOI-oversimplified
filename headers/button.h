#ifndef BUTTON_H
#define BUTTON_H

#include <SFML/Graphics.hpp>


class Button {
    sf::RectangleShape rect;
    sf::Text label;
    std::function<void()> callback;
public:
    Button(const sf::Vector2f& pos,
           const std::string& text,
           const sf::Font& font,
           std::function<void()> callback,
           const sf::Vector2f& size = {200.f, 50.f});

    void draw(sf::RenderWindow& window) const;
    bool isClicked(const sf::Vector2f& mousePos) const;
    void trigger() const;

    void setFillColor(sf::Color color);
    void setCallback(std::function<void()> callback);
    
    void setOutlineColor(const sf::Color& color);
    void setOutlineThickness(float thickness);
    void setTextColor(const sf::Color& color);

};

#endif 