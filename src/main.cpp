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

    RectangleShape attackButton;
    Text attackText;
    Region* targetRegion = nullptr;
    vector<Text> countryButtons;

    setupInterface(window, font,
                   infoPanel, infoBox, infoText,
                   overlayPanel, overlayTitle,
                   countryButtons,
                   buildButton, buildText,
                   attackButton, attackText);

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

    enum class GameState { ChoosingCountry, Playing, Fighting };
    GameState currentState = GameState::ChoosingCountry;
    int selectedCountryId = -1;

    Region* selectedRegion = nullptr;
    Country* attackerCountry = nullptr;
    Country* defenderCountry = nullptr;
    sf::Clock fightClock;
    Clock debugClock;  

    Clock clockFreezeMsg;
    bool attackModeActive = false;

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
                            int j = 0;
                            int randomIndex = rand() % countries[selectedCountryId].GetRegions().size();
                            for (auto& region : countries[selectedCountryId].GetRegions()) {
                                region.setPlayable(true);
                                if (j == randomIndex)
                                    region.setBoost();
                                j++;
                            }
                        }
                    }
                }
                continue;
            }

            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));

                
                if (attackButton.getGlobalBounds().contains(mousePos)) {
                    attackModeActive = true;
                    infoText.setString("Alege regiunea tinta...");
                    clockFreezeMsg.restart();
                    continue;
                }

                
                if (infoPanel.getGlobalBounds().contains(mousePos)) {
                    if (buildButton.getGlobalBounds().contains(mousePos)) {
                        if (selectedRegion != nullptr) {
                            try {
                                selectedRegion->buildFactory();
                                infoText.setString("Fabrica a fost construita!\nProductie activa.");
                            } catch (const FactoryBuildException& e) {
                                infoText.setString(string("Eroare la constructie:\n") + e.what());
                                clockFreezeMsg.restart();
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
                            if (attackModeActive) {
                                try {
                                    region.checkAccess();
                                    
                                    infoText.setString("Nu poti ataca regiunile proprii.");
                                } catch (const AccessDeniedException&) {
                                    targetRegion = &region;
                                    
                                    for (auto& c : countries) {
                                        for (auto& r : c.GetRegions()) {
                                            if (&r == selectedRegion) { attackerCountry = &c; break; }
                                        }
                                        if (attackerCountry) break;
                                    }
                                    for (auto& c : countries) {
                                        for (auto& r : c.GetRegions()) {
                                            if (&r == targetRegion) { defenderCountry = &c; break; }
                                        }
                                        if (defenderCountry) break;
                                    }
                                    currentState = GameState::Fighting;
                                    attackModeActive = false;
                                    fightClock.restart();
                                    debugClock.restart();
                                }
                                continue;
                            } else {
                                try {
                                    region.checkAccess();
                                    selectedRegion = &region;
                                    ostringstream s;
                                    s << "Region ID: " << region.GetID() << "\n"
                                      << "Iron: "  << region.getResources().getIron()  << "\n"
                                      << "Wood: "  << region.getResources().getWood()  << "\n"
                                      << "Gold: "  << region.getResources().getGold()  << "\n"
                                      << "Stone: " << region.getResources().getStone();
                                    infoText.setString(s.str());
                                } catch (const AccessDeniedException&) {
                                    selectedRegion = nullptr;
                                    infoText.setString("Acces interzis!\nRegiunea nu iti apartine.");
                                }
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
        window.draw(attackButton);
        window.draw(attackText);

        if (currentState == GameState::ChoosingCountry) {
            window.draw(overlayPanel);
            window.draw(overlayTitle);
            for (const auto& btn : countryButtons)
                window.draw(btn);
        }

        if (currentState == GameState::Playing && productionClock.getElapsedTime().asSeconds() >= 3.f) {
            countries[selectedCountryId].produceResources();
            productionClock.restart();
        }
        if (currentState == GameState::Playing &&
            selectedRegion != nullptr &&
            clockFreezeMsg.getElapsedTime().asSeconds() >= 3.f &&
            !attackModeActive) {
            ostringstream s;
            s << "Region ID: " << selectedRegion->GetID() << "\n"
              << "Iron: "  << selectedRegion->getResources().getIron()  << "\n"
              << "Wood: "  << selectedRegion->getResources().getWood()  << "\n"
              << "Gold: "  << selectedRegion->getResources().getGold()  << "\n"
              << "Stone: " << selectedRegion->getResources().getStone();
            infoText.setString(s.str());
        }

        if (currentState == GameState::Fighting) {
            window.clear(Color::Black);
            window.draw(backgroundSprite);

            
            if (debugClock.getElapsedTime().asSeconds() >= 2.f) {
                currentState = GameState::Playing;
                targetRegion = nullptr;
                attackerCountry = nullptr;
                defenderCountry = nullptr;
                attackModeActive = false;
            }

            window.display();
            continue;
        }

        window.display();
    }

    return 0;
}
