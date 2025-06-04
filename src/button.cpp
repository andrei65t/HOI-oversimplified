#include "../headers/button.h"

Button::Button(const sf::Vector2f& pos,
               const std::string& text,
               const sf::Font& font,
               std::function<void()> callback,
               const sf::Vector2f& size)
    : callback(std::move(callback))
{
    rect.setSize(size);
    rect.setPosition(pos);
    rect.setFillColor({80, 80, 80});

    label.setString(text);
    label.setFont(font);
    label.setCharacterSize(22);
    label.setFillColor(sf::Color::Black);

    rect.setOutlineThickness(0.f);
    rect.setOutlineColor(sf::Color::Transparent);

    
    sf::FloatRect bounds = label.getLocalBounds();
    label.setOrigin(bounds.left + bounds.width/2.f,
                     bounds.top  + bounds.height/2.f);
    label.setPosition(pos + size/2.f);
}

void Button::draw(sf::RenderWindow& window) const {
    window.draw(rect);
    window.draw(label);
}

bool Button::isClicked(const sf::Vector2f& mousePos) const {
    return rect.getGlobalBounds().contains(mousePos);
}

void Button::trigger() const {
    if (callback) callback();
}

void Button::setFillColor(sf::Color color) {
    rect.setFillColor(color);
}

void Button::setCallback(std::function<void()> callback) {
    callback = std::move(callback);
}

void Button::setOutlineColor(const sf::Color& color) {
    rect.setOutlineColor(color);
}

void Button::setOutlineThickness(float thickness) {
    rect.setOutlineThickness(thickness);
}

void Button::setTextColor(const sf::Color& color) {
    label.setFillColor(color);
}
