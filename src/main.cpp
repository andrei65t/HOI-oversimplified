#include <SFML/Graphics.hpp>
#include "../headers/countries.h"
#include "../headers/init.h"
#include "../headers/interface.h"
#include "../headers/units.h"
#include <iostream>
#include <vector>
#include <sstream>

class Button {
    sf::RectangleShape rect;
    sf::Text           label;
    std::function<void()> callback;

public:
   
    Button(const sf::Vector2f& pos,
           const std::string&  text,
           const sf::Font&     font,
           std::function<void()> cb,
           const sf::Vector2f& size = {200.f, 50.f})
        : callback(std::move(cb))
    {
        rect.setSize(size);
        rect.setPosition(pos);
        rect.setFillColor({80, 80, 80});

        label.setString(text);
        label.setFont(font);
        label.setCharacterSize(22);
       
        sf::FloatRect t = label.getLocalBounds();
        label.setOrigin(t.left + t.width/2, t.top + t.height/2);
        label.setPosition(pos + size/2.f);
    }

    void draw(sf::RenderWindow& w) const {
        w.draw(rect);
        w.draw(label);
    }
    bool isClicked(sf::Vector2f mPos) const {
        return rect.getGlobalBounds().contains(mPos);
    }
    void trigger() const { if (callback) callback(); }

   
    void setFillColor(sf::Color c)       { rect.setFillColor(c); }
    void setCallback(std::function<void()> cb) { callback = std::move(cb); }
};

std::vector<Button> battleButtons;
Army playerArmy, enemyArmy;
BattleManager bm;
sf::Text resultText;
bool battleStarted = false, battleEnded = false;

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
    Country* playerCountry = nullptr;
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
                            playerCountry     = &countries[i];
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

                
                if (currentState == GameState::Playing &&
                    infoPanel.getGlobalBounds().contains(mousePos) &&
                    buildButton.getGlobalBounds().contains(mousePos))
                {
                    if (selectedRegion)
                    {
                        try {
                            selectedRegion->buildFactory();
                            infoText.setString("Fabrica a fost construita!\nProductie activa.");
                        } catch (const FactoryBuildException& e) {
                            infoText.setString(string("Eroare la constructie:\n") + e.what());
                        }
                    }
                    else  infoText.setString("Selecteaza o regiune mai intai.");
                    clockFreezeMsg.restart();
                    continue;              
                }

                
                bool found = false;
                for (auto& country : countries) {
                    for (auto& region : country.GetRegions()) {
                        if (region.contains(mousePos)) {
                            found = true;
                            if (attackModeActive) {
                                try {
                                    region.checkAccess();
                                    
                                    infoText.setString("Nu poti ataca regiunile proprii.");
                                } catch (const AccessDeniedException&) {
                                    targetRegion = &region;
                                    
                                    attackerCountry = playerCountry;
                                    defenderCountry = nullptr;
                                    for (auto& c : countries) {
                                        for (auto& r : c.GetRegions()) {
                                            if (&r == targetRegion) { 
                                                defenderCountry = &c; 
                                                break; 
                                            }
                                        }
                                        if (defenderCountry) break;
                                    }
                                    currentState = GameState::Fighting;
                                    attackModeActive = false;

                                    int transferIndex = -1;
                                    {
                                        auto& destRegs = defenderCountry->GetRegions();
                                        for (int i = 0; i < destRegs.size(); ++i) {
                                            if (&destRegs[i] == targetRegion) {
                                                transferIndex = i;
                                                break;
                                            }
                                        }
                                    }
                                    
                                    battleButtons.clear();
                                    playerArmy = Army();
                                    enemyArmy = Army();
                                    bm.clear();
                                    resultText = sf::Text();

                                    battleButtons.emplace_back(
                                        sf::Vector2f{80,  90}, "+Soldier",  font,
                                        [&]{ playerArmy.addUnit(UnitFactory::createUnit<SoldierTraits>(1)); });
                                    
                                    battleButtons.emplace_back(
                                        sf::Vector2f{80, 160}, "+Armored",  font,
                                        [&]{ playerArmy.addUnit(UnitFactory::createUnit<ArmoredSoldierTraits>(1)); });
                                    
                                    battleButtons.emplace_back(
                                        sf::Vector2f{80, 230}, "+Tank",     font,
                                        [&]{ playerArmy.addUnit(UnitFactory::createUnit<TankTraits>(1)); });
                                    
                                    battleButtons.emplace_back(
                                        sf::Vector2f{80, 320}, "START BATTLE", font,
                                        [&, transferIndex]
                                        {
                                            if (battleStarted) return;
                                    
                                            
                                            defenderCountry->updateInformatii();
                                    
                                            int availableSoldiers = defenderCountry->getSoldati();
                                            int maxSold = std::min(availableSoldiers,
                                                                   2 + int(playerArmy.getUnits().size()) + rand()%5);
                                    
                                            int availableTanks = 0;
                                            for (const auto& reg : defenderCountry->GetRegions())
                                                availableTanks += reg.GetTanks();
                                            int maxTank = std::min(availableTanks, 1 + rand()%3);
                                    
                                            if (maxSold > 0)
                                                enemyArmy.addUnit(UnitFactory::createUnit<SoldierTraits>(maxSold));
                                            if (maxTank > 0)
                                                enemyArmy.addUnit(UnitFactory::createUnit<TankTraits>(maxTank));
                                    
                                            bm.clear();
                                            bm.addArmy(playerArmy,  true);
                                            bm.addArmy(enemyArmy,  false);
                                    
                                            bm.attach([&, transferIndex](const std::string& msg)
                                            {
                                                resultText.setFont(font);
                                                resultText.setCharacterSize(34);
                                                resultText.setFillColor(msg.find("Attacker wins!") != std::string::npos
                                                                          ? sf::Color::Green : sf::Color::Red);
                                                resultText.setString(msg);
                                                resultText.setPosition(600, 350);
                                                battleEnded = true;
                                    
                                                bool attackerWins =
                                                    (msg.find("Attacker") != std::string::npos) &&
                                                    (msg.find("wins")     != std::string::npos);
                                    
                                                if (attackerWins) {
                                                    transferRegion(*attackerCountry, *defenderCountry, targetRegion);
                                                    selectedRegion = nullptr;
                                                    targetRegion   = nullptr;
                                                }
                                            });
                                    
                                            bm.fight();
                                            battleStarted = true;
                                        });
                                    
                                    battleStarted = false;
                                    battleEnded = false;

                                    fightClock.restart();
                                    debugClock.restart();
                                }
                                continue;
                            } else {
                                try {
                                    region.checkAccess();
                                    selectedRegion = &region;
                                    ostringstream s;
                                    s << "Region ID: " << selectedRegion->GetID() << "\n"
                                    << "Iron: "  << selectedRegion->getResources().getIron()  << "\n"
                                    << "Wood: "  << selectedRegion->getResources().getWood()  << "\n"
                                    << "Gold: "  << selectedRegion->getResources().getGold()  << "\n"
                                    << "Stone: " << selectedRegion->getResources().getStone();
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
            
            if (currentState == GameState::Fighting && event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2i mpos = sf::Mouse::getPosition(window);
                sf::Vector2f m = window.mapPixelToCoords(mpos);
                for (const auto& b : battleButtons)
                    if (b.isClicked(m)) { b.trigger(); break; }

                if (battleEnded){
                    currentState = GameState::Playing;
                    battleStarted = false;
                    battleEnded = false;
                }
                    
            }
            
        }

        window.clear();
        
        if (currentState == GameState::ChoosingCountry) {
            window.draw(backgroundSprite);
            window.draw(overlayPanel);
            window.draw(overlayTitle);
            for (const auto& btn : countryButtons)
                window.draw(btn);
        } else if (currentState == GameState::Playing) {
            window.draw(backgroundSprite);
            for (const auto& country: countries)
                for (const auto& region: country.GetRegions())
                    window.draw(region.getHexShape());

                window.draw(infoPanel);
                window.draw(infoBox);
                window.draw(infoText);

                window.draw(buildButton);   
                window.draw(buildText);
                window.draw(attackButton);
                window.draw(attackText);

        } else if (currentState == GameState::Fighting) {
            window.clear(Color(22,22,30));
            window.draw(backgroundSprite);
        
           
            sf::RectangleShape panel(sf::Vector2f(340, 800));
            panel.setPosition(0,0);
            panel.setFillColor(sf::Color(35,35,50));
            window.draw(panel);
        
           
            if (!battleStarted) for (auto& b : battleButtons) b.draw(window);
        
            
            sf::Text myArmyTxt;
            myArmyTxt.setFont(font); myArmyTxt.setCharacterSize(20);
            myArmyTxt.setFillColor(sf::Color::White); myArmyTxt.setPosition(380, 120);
            std::ostringstream oss;
            oss << "Your Army:\n"; for (auto& u : playerArmy.getUnits()) oss << *u << "\n";
            myArmyTxt.setString(oss.str());
            window.draw(myArmyTxt);
        
            
            sf::Text enemyArmyTxt;
            enemyArmyTxt.setFont(font); enemyArmyTxt.setCharacterSize(20);
            enemyArmyTxt.setFillColor(sf::Color::White); enemyArmyTxt.setPosition(780, 120);
            std::ostringstream oss2;
            oss2 << "Enemy Army:\n"; for (auto& u : enemyArmy.getUnits()) oss2 << *u << "\n";
            enemyArmyTxt.setString(oss2.str());
            window.draw(enemyArmyTxt);
        
            
            if (battleEnded) window.draw(resultText);
        }
        

        window.display();
            
        
        
    }
    return 0;
}