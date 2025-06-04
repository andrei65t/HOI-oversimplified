#include "../headers/ButtonBuilder.h"
#include <cassert>   
ButtonBuilder::ButtonBuilder(): position(defaultPosition),
      size(defaultSize), labelString(""), fontPtr(nullptr),
      fillColorSet(false), outlineSet(false), textColorSet(false), callbackSet(false) {}


ButtonBuilder& ButtonBuilder::withPosition(float x, float y) {
    position = {x, y};
    return *this;
}

ButtonBuilder& ButtonBuilder::withSize(float width, float height) {
    size = {width, height};
    return *this;
}


ButtonBuilder& ButtonBuilder::withText(const std::string& text) {
    labelString = text;
    return *this;
}

ButtonBuilder& ButtonBuilder::withFont(const sf::Font& font) {
    fontPtr = &font; 
    return *this;
}

ButtonBuilder& ButtonBuilder::withFillColor(const sf::Color& color) {
    fillColor = color;
    fillColorSet = true;
    return *this;
}

ButtonBuilder& ButtonBuilder::withOutlineColor(const sf::Color& color, float thickness) {
    outlineColor     = color;
    outlineThickness = thickness;
    outlineSet       = true;
    return *this;
}

ButtonBuilder& ButtonBuilder::withTextColor(const sf::Color& color) {
    textColor    = color;
    textColorSet = true;
    return *this;
}

ButtonBuilder& ButtonBuilder::withCallback(std::function<void()> cb) {
    callback    = std::move(cb);
    callbackSet = true;
    return *this;
}

Button ButtonBuilder::build() {

    Button btn(
        position,
        labelString,
        *fontPtr,
        callback,
        size
    );
    if (fillColorSet) {
        btn.setFillColor(fillColor);
    }


    if (outlineSet) {
        btn.setOutlineColor(outlineColor);
        btn.setOutlineThickness(outlineThickness);
    }


    if (textColorSet) {
        btn.setTextColor(textColor);
    }

    return btn;  
}

