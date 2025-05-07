#include <SFML/Graphics.hpp>
#include "../headers/countries.h"
#include "../headers/init.h"
#include "../headers/interface.h"
#include <iostream>
#include <vector>

using namespace sf;
using namespace std;

std::vector<Country> countries;

int main() {
    RenderWindow window(VideoMode(1200, 800), "Countries", Style::Titlebar | Style::Close);
    initializare(window, countries);

    Font font;
    if (!font.loadFromFile("resources/arial.ttf")) {
        cerr << "Error loading font!" << endl;
        return -1;
    }

    RectangleShape infoPanel, infoBox, overlayPanel, buildButton;
    Text infoText, overlayTitle, buildText;
    sf::Clock productionClock;

    vector<Text> countryButtons;

    setupInterface(window, font,infoPanel, infoBox, infoText,overlayPanel, overlayTitle, countryButtons,buildButton, buildText);


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

    enum class GameState { ChoosingCountry, Playing };
    GameState currentState = GameState::ChoosingCountry;
    int selectedCountryId = -1;

    Region* selectedRegion = nullptr;


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
                            infoText.setString("Playing as " + countries[selectedCountryId].GetName());
                            countries[selectedCountryId].outline(Color::White);
                            for (auto& region : countries[selectedCountryId].GetRegions()) {
                                region.setPlayable(true);
                            }
                            
                            
                        }
                    }
                }
                continue;
            }

            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));
            
                if (infoPanel.getGlobalBounds().contains(mousePos)) {
                    if (buildButton.getGlobalBounds().contains(mousePos)) {
                        if (selectedRegion != nullptr) {
                            try {
                                selectedRegion->buildFactory();
                                infoText.setString("Fabrica a fost construita!\nProductie activa.");
                            } catch (const FactoryBuildException& e) {
                                infoText.setString("Eroare la constructie:\n" + string(e.what()));
                            }
                        } else {
                            infoText.setString("Selecteaza o regiune mai intai.");
                        }
                    }
            
                    continue;  
                }
            
                bool found = false;
                for (auto& country : countries) {
                    for (auto& region : country.GetRegions()) {
                        if (region.getHexShape().getGlobalBounds().contains(mousePos)) {
                            found = true;
                            try {
                                region.checkAccess();
                                selectedRegion = &region; 
            
                                std::ostringstream s;
                                s << "Region ID: " << region.GetID() << "\n";
                                s << "Iron: "  << region.getResources().getIron() << "\n";
                                s << "Wood: "  << region.getResources().getWood() << "\n";
                                s << "Gold: "  << region.getResources().getGold() << "\n";
                                s << "Stone: " << region.getResources().getStone();
                                infoText.setString(s.str());
            
                            } catch (const AccessDeniedException&) {
                                selectedRegion = nullptr; 
                                infoText.setString("Acces interzis!\nRegiunea nu iti\napartine.");
                            }
                            break;
                        }
                    }
                    if (found) break;
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
        window.draw(buildButton);
        window.draw(buildText);

        if (currentState == GameState::ChoosingCountry) {
            window.draw(overlayPanel);
            window.draw(overlayTitle);
            for (const auto& button : countryButtons)
                window.draw(button);
        }

        if (currentState == GameState::Playing && productionClock.getElapsedTime().asSeconds() >= 3.f) {
            countries[selectedCountryId].produceResources();
            productionClock.restart();
        }
        
        window.display();
    }
    return 0;
}
