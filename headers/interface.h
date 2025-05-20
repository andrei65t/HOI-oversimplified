#ifndef INTERFACE_H
#define INTERFACE_H
#pragma once
#include <SFML/Graphics.hpp>
#include "countries.h"

void setupInterface(sf::RenderWindow& window, sf::Font& font,
    sf::RectangleShape& infoPanel, sf::RectangleShape& infoBox, sf::Text& infoText,
    sf::RectangleShape& overlayPanel, sf::Text& overlayTitle, std::vector<sf::Text>& countryButtons,
    sf::RectangleShape& buildButton, sf::Text& buildText, sf::RectangleShape& attackButton, sf::Text& attackText);


#endif
