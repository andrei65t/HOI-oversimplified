#ifndef BUTTONBUILDER_H
#define BUTTONBUILDER_H

#include <SFML/Graphics.hpp>
#include <functional>
#include <memory>        
#include "button.h"      

class ButtonBuilder {
        
    sf::Vector2f position;
    sf::Vector2f size;
    std::string labelString;
    const sf::Font* fontPtr;       
    sf::Color fillColor;
    bool fillColorSet;     
    sf::Color outlineColor;
    float outlineThickness;
    bool outlineSet;       
    sf::Color textColor;
    bool textColorSet;
    std::function<void()> callback;
    bool callbackSet;


    const sf::Vector2f defaultPosition{0.f, 0.f};
    const sf::Vector2f defaultSize{200.f, 50.f};

public:
    ButtonBuilder();
    ~ButtonBuilder() = default;

    
    ButtonBuilder& withPosition(float x, float y);
    ButtonBuilder& withSize(float width, float height);
    ButtonBuilder& withText(const std::string& text);
    ButtonBuilder& withFont(const sf::Font& font);
    ButtonBuilder& withFillColor(const sf::Color& color);
    ButtonBuilder& withOutlineColor(const sf::Color& color, float thickness);
    ButtonBuilder& withTextColor(const sf::Color& color);
    ButtonBuilder& withCallback(std::function<void()> callback);


    Button build();

};

#endif 
