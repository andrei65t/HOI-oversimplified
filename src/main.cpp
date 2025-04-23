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
    Color(178, 34, 34),    // Firebrick
    Color(0, 139, 139),    // Dark Cyan
    Color(72, 61, 139),    // Dark Slate Blue
    Color(218, 165, 32)    // Goldenrod
};

void initializare(RenderWindow& window) {
    srand(time(0));
    float hexRadius = 70.f;
    float hexWidth = hexRadius * sqrt(3.f);  // point-to-point horizontal
    float hexHeight = hexRadius * 2.f;       // point-to-point vertical
    float verticalSpacing = hexHeight * 0.866f;
    float horizontalSpacing = hexWidth * 0.866f; // add small gap between hexes

    int cols = 6;
    int rows = 4;

    float gridWidth = horizontalSpacing * (cols - 1) + hexWidth;
    float gridHeight = verticalSpacing * (rows - 1) + hexHeight;
    float offsetX = (window.getSize().x - gridWidth) / 2.f - 150.f; // shift left by 150px

    float offsetY = (window.getSize().y - gridHeight) / 2.f;

    vector<Region> tmpRegions[4];

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            float x = offsetX + j * horizontalSpacing;
            float y = offsetY + i * verticalSpacing;
            if (j % 2 == 1) y += verticalSpacing / 2.f;

            int countryId = 0;
            if (i < 2 && j >= 3) countryId = 1;         // top-right
            else if (i >= 2 && j < 3) countryId = 2;     // bottom-left
            else if (i >= 2 && j >= 3) countryId = 3;    // bottom-right
            // else remains countryId = 0 (top-left)

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

    enum class GameState { ChoosingCountry, Playing };
    GameState currentState = GameState::ChoosingCountry;
    int selectedCountryId = -1;

    RectangleShape overlayPanel(Vector2f(500.f, 400.f));
    overlayPanel.setFillColor(Color(0, 0, 0, 200));  // semi-transparent black
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

    
    RectangleShape infoPanel(Vector2f(1200.f, 50.f));
    infoPanel.setFillColor(Color::Black);
    infoPanel.setPosition(0, 0);

    Text infoText;
    infoText.setFont(font);
    infoText.setCharacterSize(24);
    infoText.setFillColor(Color::White);
    infoText.setPosition(10.f, 10.f);
    infoText.setString("");

    Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("resources/background.png")) {
        cerr << "Error loading background image!" << endl;
        return -1;
    }
    Sprite backgroundSprite(backgroundTexture);
    backgroundSprite.setScale(
        window.getSize().x / static_cast<float>(backgroundTexture.getSize().x),
        window.getSize().y / static_cast<float>(backgroundTexture.getSize().y)
    );

    bool mousePressed = false;

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (currentState == GameState::ChoosingCountry) {
                if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                    Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));
                    for (int i = 0; i < countryButtons.size(); i++) {
                        if (countryButtons[i].getGlobalBounds().contains(mousePos)) {
                            selectedCountryId = i;
                            currentState = GameState::Playing;
                            infoText.setString("Playing as " + names[selectedCountryId]);
                            cerr << "Selected country: " << names[selectedCountryId] << endl;
                        }
                    }
                }
                continue;
            }
            if (currentState == GameState::Playing){
                if (event.type == Event::MouseButtonPressed) {
                    if (event.mouseButton.button == Mouse::Left && !mousePressed) {
                        mousePressed = true;
                        Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));
        
                        for (const auto& country : countries)
                            for (const auto& region : country.GetRegions())
                                if (region.contains(mousePos)) {
                                region.afisareRegiune();
                                infoText.setString(region.afisareRegiune());
                                }

                    }
                }
                if (event.type == Event::MouseButtonReleased)
                    if (event.mouseButton.button == Mouse::Left && mousePressed)
                        mousePressed = false;
                if (event.type == Event::Closed)
                    window.close();
            }
        }        
            window.clear();
            window.draw(backgroundSprite);

            for (const auto& country : countries) {
                for (const auto& region : country.GetRegions()) {
                    window.draw(region.getHexShape());
                }
            }
        window.draw(infoPanel);
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
