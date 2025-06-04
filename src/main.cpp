#include <SFML/Graphics.hpp>
#include "../headers/countries.h"
#include "../headers/init.h"
#include "../headers/interface.h"
#include "../headers/units.h"
#include "../headers/button.h"
#include "../headers/ButtonBuilder.h"
#include "../headers/command.h"      
#include <iostream>
#include <vector>
#include <sstream>


std::vector<Button> battleButtons;
Army playerArmy, enemyArmy;
BattleManager bm;
sf::Text resultText;
bool battleStarted = false, battleEnded = false;
sf::Clock revealClock;
bool pendingReveal = false;
std::unique_ptr<Command> martialLawCmd;
bool martialLawUsed = false;

bool labourEnabled = false; 
sf::RectangleShape labourButton; 
sf::Text labourText; 
bool pierdut=false;
sf::Clock aiAttackClock;

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
                   attackButton, attackText,
                   labourButton, labourText);

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

    enum class GameState { ChoosingCountry, Playing, Fighting , EndPopularity, Winner , Loser};
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
    bool useRandomized = false;
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
                            playerCountry = &countries[i];
                            aiAttackClock.restart();
                            playerCountry->setPc(true);
                            currentState = GameState::Playing;
                            infoText.setString("Joci cu " + countries[selectedCountryId].GetName());
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
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::R) {
                useRandomized = !useRandomized;
                if (useRandomized) {
                    bm.setStrategy(std::make_unique<RandomizedStrategy>());
                    cerr<< "Using Randomized Strategy" << endl;
                } else {
                    bm.setStrategy(std::make_unique<ClassicBattleStrategy>());
                    cerr<< "Using Classic Strategy" << endl;
                }
            }

            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));

                if (labourButton.getGlobalBounds().contains(mousePos)) {

                    if (!labourEnabled) {
                        playerCountry->getPopularity().decrease(30);
                        labourButton.setFillColor(Color::Green); 
                        labourText.setString("Labour ON");
                        labourEnabled = true;
                        playerCountry->setlabour(true);
                    } else {
                        playerCountry->getPopularity().increase(25);
                        labourButton.setFillColor(Color::Red); 
                        labourText.setString("Labour OFF");
                        labourEnabled = false;
                        playerCountry->setlabour(false);
                    }
                }
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
                            playerCountry->increasePopularity(5);
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


                                    martialLawCmd = std::make_unique<MartialLawCommand>(*playerCountry, playerArmy);
                                    battleButtons.push_back(
                                        ButtonBuilder()
                                            .withPosition(80.f, 20.f)
                                            .withSize(200.f, 50.f)
                                            .withText("Legea Martiala")
                                            .withFont(font)
                                            .withCallback([&]() {
                                                if (!martialLawUsed) {
                                                    martialLawCmd->execute();
                                                    martialLawUsed = true;
                                                }
                                            })
                                            .build()
                                    );

                                    battleButtons.push_back(
                                        ButtonBuilder()
                                            .withPosition(80.f,  90.f)
                                            .withSize(200.f, 50.f)           
                                            .withText("+Infanterie")
                                            .withFont(font)
                                            
                                            .withCallback([&](){
                                                Resources rCost = SoldierTraits::cost(1);
                                                Resources total = playerCountry->getTotalResources();
                                                if (total.getIron() < rCost.getIron() ||
                                                    total.getGold() < rCost.getGold()) {
                                                    return;
                                                }else{
                                                playerCountry->consumeResourcesFromRegions(rCost);
                                                playerArmy.addUnit(UnitFactory::createUnit<SoldierTraits>(1));}
                                            })
                                            .build()
                                    );
                                    
                                    
                                    battleButtons.push_back(
                                        ButtonBuilder()
                                            .withPosition(80.f, 160.f)
                                            .withSize(200.f, 50.f)
                                            .withText("+Artilerie")
                                            .withFont(font)
                                            
                                            
                                            .withCallback([&](){
                                                Resources rCost = ArmoredSoldierTraits::cost(1);
                                                Resources total = playerCountry->getTotalResources();
                                                if (total.getIron() < rCost.getIron() ||
                                                    total.getGold() < rCost.getGold()) {
                                                    return;
                                                }else{
                                                playerCountry->consumeResourcesFromRegions(rCost);
                                                playerArmy.addUnit(UnitFactory::createUnit<ArmoredSoldierTraits>(1));}
                                            })
                                            .build()
                                    );
                                    
                                    
                                    battleButtons.push_back(
                                        ButtonBuilder()
                                            .withPosition(80.f, 230.f)
                                            .withSize(200.f, 50.f)
                                            .withText("+Tanc")
                                            .withFont(font)
                                            
                                            .withCallback([&](){
                                                Resources rCost = TankTraits::cost(1);
                                                Resources total = playerCountry->getTotalResources();
                                                if (total.getIron() < rCost.getIron() ||
                                                total.getGold() < rCost.getGold()) {
                                                return;
                                            }else{
                                                playerCountry->consumeResourcesFromRegions(rCost);
                                                playerArmy.addUnit(UnitFactory::createUnit<TankTraits>(1));}
                                            })
                                            .build()
                                    );
                                    
                                    
                                    battleButtons.push_back(
                                        ButtonBuilder()
                                            .withPosition(80.f, 320.f)
                                            .withSize(200.f, 50.f)
                                            .withText("START BATALIE")
                                            .withFont(font)
                                            .withCallback([&, transferIndex](){
                                                
                                            if (battleStarted) return;

                                            defenderCountry->updateInformatii();
                                            
                                            int availableSoldiers = defenderCountry->getSoldati();
                                            int maxSold = availableSoldiers/1200;
                                            int availableTanks = 0;
                                            for (const auto& reg : defenderCountry->GetRegions())
                                                availableTanks += reg.GetTanks();
                                            cerr<< "AvailableSoldiers: " << availableSoldiers
                                                 << ", AvailableTanks: " << availableTanks << endl;
                                            int maxTank = std::min(availableTanks+rand()%3, 1 + rand()%3);
                                            cerr << "MaxSold: " << maxSold << ", MaxTank: " << maxTank << endl;
                                            
                                            if (maxSold > 0)
                                                enemyArmy.addUnit(UnitFactory::createUnit<SoldierTraits>(maxSold));
                                            if (maxTank > 0)
                                                enemyArmy.addUnit(UnitFactory::createUnit<TankTraits>(maxTank));
                                            
                                            battleStarted  = true;
                                            pendingReveal  = true;
                                            revealClock.restart();
                                            
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
                                                    playerCountry->increasePopularity(10);
                                                    transferRegion(*attackerCountry, *defenderCountry, targetRegion);
                                                    selectedRegion = nullptr;
                                                    targetRegion   = nullptr;
                                                } else{
                                                    playerCountry->decreasePopularity(8);
                                                }
                                                
                                            });
                                    
                                            battleStarted = true;
                                        })
                                        .build()
                                    );
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
            if (currentState == GameState::Playing && playerCountry->getPopularity().get() < 20)
            {
                currentState = GameState::EndPopularity;
            }
            if (currentState == GameState::Playing && playerCountry->GetRegions().size() == 24) {
                currentState = GameState::Winner;
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
        if (currentState == GameState::Playing && productionClock.getElapsedTime().asSeconds() >= 2.f) {
            countries[selectedCountryId].produceResources();
            productionClock.restart();
        }
        if (currentState == GameState::Playing &&
            selectedRegion != nullptr &&
            clockFreezeMsg.getElapsedTime().asSeconds() >= 2.f &&
            !attackModeActive) {
            ostringstream s;
            s << "Region ID: " << selectedRegion->GetID() << "\n"
              << "Iron: "  << selectedRegion->getResources().getIron()  << "\n"
              << "Wood: "  << selectedRegion->getResources().getWood()  << "\n"
              << "Gold: "  << selectedRegion->getResources().getGold()  << "\n"
              << "Stone: " << selectedRegion->getResources().getStone();
            infoText.setString(s.str());
        }

        if (currentState == GameState::Playing && 
            !battleStarted && 
            !pendingReveal && 
            !battleEnded && 
            aiAttackClock.getElapsedTime().asSeconds() >= 10.f)
        {
            aiAttackClock.restart();
        
            
            std::vector<Country*> aiCountriesAttacker;
            std::vector<Country*> aiCountriesDefender;
            for (auto& c : countries)
                if (&c != playerCountry)
                aiCountriesAttacker.push_back(&c);
        
            for (auto& c : countries)
                aiCountriesDefender.push_back(&c);

            Country* attacker = nullptr;
            Country* defender = nullptr;
            
            do {
                attacker = aiCountriesAttacker[rand() % aiCountriesAttacker.size()];
                defender = aiCountriesDefender[rand() % aiCountriesDefender.size()];
            } while (attacker == defender || defender->GetRegions().empty());
            
            if (defender == playerCountry) {
                


                attackerCountry = attacker;
                defenderCountry = defender;
                currentState = GameState::Fighting;
                attackModeActive = false;
                targetRegion = &defender->GetRegions()[rand() % defender->GetRegions().size()];
                int transferIndex = -1;
                    auto& destRegs = defenderCountry->GetRegions();
                    for (int i = 0; i < destRegs.size(); ++i) {
                        if (&destRegs[i] == targetRegion) {
                            transferIndex = i;
                            break;
                        }
                    }
                Region*  tgtPtr = targetRegion;  
                Country* atkPtr = attacker;       
                Country* defPtr = defender;       
                cerr << transferIndex << endl;
                battleButtons.clear();
                playerArmy = Army();
                enemyArmy = Army();
                bm.clear();
                resultText = sf::Text();

                martialLawCmd = std::make_unique<MartialLawCommand>(*playerCountry, playerArmy);
                battleButtons.push_back(
                    ButtonBuilder()
                        .withPosition(80.f, 20.f)
                        .withSize(200.f, 50.f)
                        .withText("Legea Martiala")
                        .withFont(font)
                        .withCallback([&]() {
                            if (!martialLawUsed) {
                                martialLawCmd->execute();
                                martialLawUsed = true;
                            }
                        })
                        .build()
                );

                battleButtons.push_back(
                    ButtonBuilder()
                        .withPosition(80.f, 90.f)
                        .withSize(200.f, 50.f)
                        .withText("+Infanterie")
                        .withFont(font)
                        .withCallback([&]() {
                            Resources rCost = SoldierTraits::cost(1);
                            Resources total = playerCountry->getTotalResources();
                            if (total.getIron() < rCost.getIron() ||
                                total.getGold() < rCost.getGold()) {
                                return;
                            } else {
                                playerCountry->consumeResourcesFromRegions(rCost);
                                playerArmy.addUnit(UnitFactory::createUnit<SoldierTraits>(1));
                            }
                        })
                        .build()
                );

                battleButtons.push_back(
                    ButtonBuilder()
                        .withPosition(80.f, 160.f)
                        .withSize(200.f, 50.f)
                        .withText("+Artilerie")
                        .withFont(font)
                        .withCallback([&]() {
                            Resources rCost = ArmoredSoldierTraits::cost(1);
                            Resources total = playerCountry->getTotalResources();
                            if (total.getIron() < rCost.getIron() ||
                                total.getGold() < rCost.getGold()) {
                                return;
                            } else {
                                playerCountry->consumeResourcesFromRegions(rCost);
                                playerArmy.addUnit(UnitFactory::createUnit<ArmoredSoldierTraits>(1));
                            }
                        })
                        .build()
                );

                battleButtons.push_back(
                    ButtonBuilder()
                        .withPosition(80.f, 230.f)
                        .withSize(200.f, 50.f)
                        .withText("+Tanc")
                        .withFont(font)
                        .withCallback([&]() {
                            Resources rCost = TankTraits::cost(1);
                            Resources total = playerCountry->getTotalResources();
                            if (total.getIron() < rCost.getIron() ||
                                total.getGold() < rCost.getGold()) {
                                return;
                            } else {
                                playerCountry->consumeResourcesFromRegions(rCost);
                                playerArmy.addUnit(UnitFactory::createUnit<TankTraits>(1));
                            }
                        })
                        .build()
                );

                battleButtons.push_back(
                    ButtonBuilder()
                        .withPosition(80.f, 320.f)
                        .withSize(200.f, 50.f)
                        .withText("START BATALIE")
                        .withFont(font)
                        .withCallback([=]() mutable {
                            if (battleStarted) return;

                            defenderCountry->updateInformatii();

                            int availableSoldiers = attackerCountry->getSoldati();
                            int maxSold = availableSoldiers / 1200;
                            int availableTanks = 0;
                            for (const auto& reg : attackerCountry->GetRegions())
                                availableTanks += reg.GetTanks();
                            cerr << "AvailableSoldiers: " << availableSoldiers
                                 << ", AvailableTanks: " << availableTanks << endl;
                            int maxTank = std::min(availableTanks + rand() % 3, 1 + rand() % 3);
                            cerr << "MaxSold: " << maxSold << ", MaxTank: " << maxTank << endl;

                            if (maxSold > 0)
                                enemyArmy.addUnit(UnitFactory::createUnit<SoldierTraits>(maxSold));
                            if (maxTank > 0)
                                enemyArmy.addUnit(UnitFactory::createUnit<TankTraits>(maxTank));

                            battleStarted = true;
                            pendingReveal = true;
                            revealClock.restart();

                            bm.clear();
                            bm.addArmy(playerArmy, false);
                            bm.addArmy(enemyArmy, true);

                            bm.attach([&, transferIndex](const std::string& msg) {
                                resultText.setFont(font);
                                resultText.setCharacterSize(34);
                                resultText.setFillColor(msg.find("Defender wins!") != std::string::npos
                                                          ? sf::Color::Green
                                                          : sf::Color::Red);
                                resultText.setString(msg);
                                resultText.setPosition(600, 350);
                                battleEnded = true;

                                bool aiWin = (msg.find("Attacker") != std::string::npos);
                                cerr << aiWin << endl;
                                if (aiWin) {
                                    cerr << "Ai pierdut batalia!" << endl;
                                    cerr << playerCountry->GetRegions().size() << endl;
                                    if(playerCountry->GetRegions().size() == 1) {
                                        pierdut = true;
                                        currentState = GameState::Loser;
                                    }
                                    else{
                                            playerCountry->decreasePopularity(15);
                                            transferRegion(*atkPtr, *defPtr, tgtPtr);
                                            selectedRegion = nullptr;
                                            targetRegion = nullptr;
                                    }
                                    
                                    
                                } else {
                                    playerCountry->increasePopularity(10);
                                }
                            });

                            battleStarted = true;
                        })
                        .build()
                );
                battleStarted = false;
                battleEnded = false;

                fightClock.restart();
                debugClock.restart();
            } else {
                Region* targetRegion = &defender->GetRegions()[rand() % defender->GetRegions().size()];
                bool attackerWins = rand() % 2 == 0;
            
                if (attackerWins) {
                    cerr << attacker->GetName() << " wins and takes region " << targetRegion->GetID() << endl;
                    transferRegion(*attacker, *defender, targetRegion);
                } else {
                    cerr << defender->GetName() << " defends successfully.\n";
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

                sf::Text popText;
                popText.setFont(font);
                popText.setCharacterSize(22);
                popText.setFillColor(sf::Color::Red);
                popText.setPosition(300, window.getSize().y - 50);

                std::ostringstream oss;
                oss << "Popularitate: " << playerCountry->getPopularity();
                popText.setString(oss.str());
                window.draw(popText);

                window.draw(labourButton);
                window.draw(labourText);

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
            myArmyTxt.setFillColor(sf::Color::Black); myArmyTxt.setPosition(380, 120);
            std::ostringstream oss;
            oss << "Armata ta:\n"; for (auto& u : playerArmy.getUnits()) oss << *u << "\n";
            myArmyTxt.setString(oss.str());
            window.draw(myArmyTxt);
        
            
            sf::Text enemyArmyTxt;
            enemyArmyTxt.setFont(font); enemyArmyTxt.setCharacterSize(20);
            enemyArmyTxt.setFillColor(sf::Color::Black); enemyArmyTxt.setPosition(780, 120);
            std::ostringstream oss2;
            oss2 << "Armata Adversa:\n"; for (auto& u : enemyArmy.getUnits()) oss2 << *u << "\n";
            enemyArmyTxt.setString(oss2.str());
            window.draw(enemyArmyTxt);
            
            if (pendingReveal && revealClock.getElapsedTime() >= seconds(3.f)) {
                pendingReveal = false;
                bm.fight();
            }
            if (battleEnded) {
                window.draw(resultText);
            }
            if (battleEnded) {
                if(pierdut==false){
                currentState = GameState::Playing;
                battleStarted = false;
                battleEnded = false;
                pendingReveal = false;
                martialLawUsed = false;
                playerArmy = Army();
                enemyArmy = Army();
                bm.clear();
                resultText = sf::Text();
                aiAttackClock.restart();
                }
                else{
                    goto debug;
                }
            }
        
        }else if (currentState == GameState::EndPopularity) {
            window.clear(Color(22,22,30));
            
            Text endText;
            endText.setFont(font);
            endText.setCharacterSize(30);
            endText.setFillColor(Color::Red);
            endText.setString("                     AI PIERDUT!\n\nPopularitatea ta in randul oamenilor a ajuns sub 20%.\nAceasta a provocat o revolta generala.");
            endText.setPosition(300, 300);
            window.draw(endText);

            RectangleShape exitButton(Vector2f(200, 50));
            exitButton.setFillColor(Color::Red);
            exitButton.setPosition(500, 500);
            window.draw(exitButton);

            Text exitText;
            exitText.setFont(font);
            exitText.setCharacterSize(24);
            exitText.setFillColor(Color::White);
            exitText.setString("EXIT GAME");
            exitText.setPosition(530, 510);
            window.draw(exitText);

            Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));
            if (Mouse::isButtonPressed(Mouse::Left) && exitButton.getGlobalBounds().contains(mousePos)) {
                window.close();
            }
        }else if( currentState == GameState::Winner) {
            window.clear(Color(22,22,30));
            
            Text winText;
            winText.setFont(font);
            winText.setCharacterSize(30);
            winText.setFillColor(Color::Green);
            winText.setString("                     AI CASTIGAT!\n\nAi reusit sa cuceresti toate regiunile.");
            winText.setPosition(300, 300);
            window.draw(winText);

            RectangleShape exitButton(Vector2f(200, 50));
            exitButton.setFillColor(Color::Green);
            exitButton.setPosition(500, 500);
            window.draw(exitButton);

            Text exitText;
            exitText.setFont(font);
            exitText.setCharacterSize(24);
            exitText.setFillColor(Color::White);
            exitText.setString("EXIT GAME");
            exitText.setPosition(530, 510);
            window.draw(exitText);

            Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));
            if (Mouse::isButtonPressed(Mouse::Left) && exitButton.getGlobalBounds().contains(mousePos)) {
                window.close();
            }
        }else if( currentState == GameState::Loser) {
            debug:
            window.clear(Color(22,22,30));
            
            Text loseText;
            loseText.setFont(font);
            loseText.setCharacterSize(30);
            loseText.setFillColor(Color::Red);
            loseText.setString("                     AI PIERDUT!\n\nAi pierdut toate regiunile si nu mai ai nicio sansa de a reveni.");
            loseText.setPosition(300, 300);
            window.draw(loseText);

            RectangleShape exitButton(Vector2f(200, 50));
            exitButton.setFillColor(Color::Red);
            exitButton.setPosition(500, 500);
            window.draw(exitButton);

            Text exitText;
            exitText.setFont(font);
            exitText.setCharacterSize(24);
            exitText.setFillColor(Color::White);
            exitText.setString("EXIT GAME");
            exitText.setPosition(530, 510);
            window.draw(exitText);

            Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));
            if (Mouse::isButtonPressed(Mouse::Left) && exitButton.getGlobalBounds().contains(mousePos)) {
                window.close();
            }
        }

        window.display();
            
        
        
    }
    return 0;
}