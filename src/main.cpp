#include <iostream>
#include <SFML/Graphics.hpp>
#include "../headers/countries.h"
#include <vector>
#include <cmath>
using namespace std;
using namespace sf;

vector<Region> regions;
vector<Country> countries;

string names[4] = {"Romania", "Bulgaria", "Serbia", "Ungaria"};
Color colors[4] = {
    Color(178, 34, 34),
    Color(0, 139, 139),
    Color(72, 61, 139),
    Color(218, 165, 32)
};

void initializare(RenderWindow& window) {
    srand(time(0));
    float hexRadius = 70.f;
    float hexWidth = hexRadius * sqrt(3.f);
    float hexHeight = hexRadius * 2.f;
    float verticalSpacing = hexHeight * 0.866f + 4.f;
    float horizontalSpacing = hexWidth * 0.866f + 3.f;

    int cols = 6, rows = 4;
    float gridWidth = horizontalSpacing * (cols - 1) + hexWidth;
    float gridHeight = verticalSpacing * (rows - 1) + hexHeight;
    float offsetX = (window.getSize().x - gridWidth) / 2.f - 150.f;
    float offsetY = (window.getSize().y - gridHeight) / 2.f;

    vector<Region> tmpRegions[4];

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            float x = offsetX + j * horizontalSpacing;
            float y = offsetY + i * verticalSpacing;
            if (j % 2 == 1) y += verticalSpacing / 2.f;

            int countryId = 0;
            if (i < 2 && j >= 3) countryId = 1;
            else if (i >= 2 && j < 3) countryId = 2;
            else if (i >= 2 && j >= 3) countryId = 3;

            HexagonShape hex(colors[countryId], Vector2f(x, y), Color::Black);
            Resources res(rand() % 1000 + 100, rand() % 1000 + 100, rand() % 1000 + 100, rand() % 1000 + 100);
            tmpRegions[countryId].push_back(Region(i * cols + j, res, hex));
        }
    }

    for (int i = 0; i < 4; i++) {
        countries.push_back(Country(i, names[i], tmpRegions[i]));
    }
}

int main() {
    RenderWindow window(VideoMode(1200, 800), "Countries", Style::Titlebar | Style::Close);
    initializare(window);
    Font font;
    if (!font.loadFromFile("resources/arial.ttf")) {
        cerr << "Error loading font!" << endl;
        return -1;
    }

    sf::RectangleShape infoPanel;
    sf::RectangleShape infoBox;
    sf::Text infoText;

    infoPanel.setSize(Vector2f(300.f, window.getSize().y));
    infoPanel.setFillColor(Color(20, 20, 20, 230));
    infoPanel.setPosition(window.getSize().x - 300.f, 0.f);

    infoBox.setSize(Vector2f(280.f, 300.f));
    infoBox.setFillColor(Color(40, 40, 40, 200));
    infoBox.setOutlineColor(Color(60, 60, 60, 255));
    infoBox.setOutlineThickness(2.f);
    infoBox.setPosition(infoPanel.getPosition().x + 10.f, 20.f);

    infoText.setFont(font);
    infoText.setCharacterSize(20);
    infoText.setFillColor(Color(220, 220, 220));
    infoText.setPosition(infoBox.getPosition().x + 15.f, infoBox.getPosition().y + 15.f);
    infoText.setLineSpacing(1.5f);


    enum class GameState { ChoosingCountry, Playing };
    GameState currentState = GameState::ChoosingCountry;
    int selectedCountryId = -1;

    RectangleShape overlayPanel(Vector2f(500.f, 400.f));
    overlayPanel.setFillColor(Color(0, 0, 0, 200));
    overlayPanel.setOrigin(250.f, 200.f);
    overlayPanel.setPosition(window.getSize().x / 2.f, window.getSize().y / 2.f);

    Text overlayTitle;
    overlayTitle.setFont(font);
    overlayTitle.setCharacterSize(36);
    overlayTitle.setFillColor(Color::White);
    overlayTitle.setString("Alege o tara:");
    overlayTitle.setPosition(overlayPanel.getPosition().x - 100.f, overlayPanel.getPosition().y - 160.f);

    vector<Text> countryButtons;
    for (int i = 0; i < 4; i++) {
        Text button;
        button.setFont(font);
        button.setString(names[i]);
        button.setCharacterSize(30);
        button.setFillColor(colors[i]);
        button.setPosition(overlayPanel.getPosition().x - 100.f, overlayPanel.getPosition().y - 80.f + i * 60.f);
        countryButtons.push_back(button);
    }


    Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("resources/background.png")) {
        cerr << "Error loading background image!" << endl;
        return -1;
    }
    Sprite backgroundSprite(backgroundTexture);
    backgroundSprite.setScale(
        window.getSize().x / float(backgroundTexture.getSize().x),
        window.getSize().y / float(backgroundTexture.getSize().y)
    );

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();

            if (currentState == GameState::ChoosingCountry) {
                if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                    Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));
                    for (int i = 0; i < countryButtons.size(); i++) {
                        if (countryButtons[i].getGlobalBounds().contains(mousePos)) {
                            selectedCountryId = i;
                            currentState = GameState::Playing;
                            infoText.setString("Playing as " + names[selectedCountryId]);
                            countries[selectedCountryId].outline(Color::White);
                        }
                    }
                }
                continue;
            }

            
            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));
                for (const auto& country : countries) {
                    for (const auto& region : country.GetRegions()) {
                        if (region.getHexShape().getGlobalBounds().contains(mousePos)) {
                            std::ostringstream infoStream;
                            infoStream << "Region ID: " << region.GetID() << "\n";
                            infoStream << "Iron: " << region.getResources().getIron() << "\n";
                            infoStream << "Wood: " << region.getResources().getWood() << "\n";
                            infoStream << "Gold: " << region.getResources().getGold() << "\n";
                            infoStream << "Stone: " << region.getResources().getStone();
                            infoText.setString(infoStream.str());
                        }
                    }
                }
            }
        }

        
        window.clear();
        window.draw(backgroundSprite);

        for (const auto& country : countries)
            for (const auto& region : country.GetRegions())
                window.draw(region.getHexShape());

        window.draw(infoPanel);
        window.draw(infoBox);
        window.draw(infoText);

        if (currentState == GameState::ChoosingCountry) {
            window.draw(overlayPanel);
            window.draw(overlayTitle);
            for (const auto& button : countryButtons)
                window.draw(button);
        }

        window.display();
    }
    return 0;
}
