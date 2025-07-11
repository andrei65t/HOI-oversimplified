#include "interface.h"

using namespace sf;
using namespace std;

extern string names[4];
extern Color colors[4];

void setupInterface(sf::RenderWindow& window, sf::Font& font,
    sf::RectangleShape& infoPanel, sf::RectangleShape& infoBox, sf::Text& infoText,
    sf::RectangleShape& overlayPanel, sf::Text& overlayTitle, std::vector<sf::Text>& countryButtons,
    sf::RectangleShape& buildButton, sf::Text& buildText, sf::RectangleShape& attackButton, sf::Text& attackText,
    sf::RectangleShape& labourButton,sf::Text& labourText) {

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

    overlayPanel.setSize(Vector2f(500.f, 400.f));
    overlayPanel.setFillColor(Color(0, 0, 0, 200));
    overlayPanel.setOrigin(250.f, 200.f);
    overlayPanel.setPosition(window.getSize().x / 2.f, window.getSize().y / 2.f);

    overlayTitle.setFont(font);
    overlayTitle.setCharacterSize(36);
    overlayTitle.setFillColor(Color::White);
    overlayTitle.setString("Alege o tara:");
    overlayTitle.setPosition(overlayPanel.getPosition().x - 100.f, overlayPanel.getPosition().y - 160.f);

    buildButton.setSize(Vector2f(280.f, 40.f));
    buildButton.setFillColor(Color(70, 70, 70));
    buildButton.setOutlineThickness(2.f);
    buildButton.setOutlineColor(Color(100, 100, 100));
    buildButton.setPosition(infoBox.getPosition().x, infoBox.getPosition().y + infoBox.getSize().y + 10.f);

    buildText.setFont(font);
    buildText.setCharacterSize(18);
    buildText.setFillColor(Color::White);
    buildText.setString("Construieste fabrica");
    buildText.setPosition(buildButton.getPosition().x + 20.f, buildButton.getPosition().y + 5.f);

    attackButton.setSize(Vector2f(280.f, 40.f));
    attackButton.setFillColor(Color(90, 90, 90));
    attackButton.setOutlineThickness(2.f);
    attackButton.setOutlineColor(Color(120, 120, 120));
    attackButton.setPosition(infoBox.getPosition().x, infoBox.getPosition().y + infoBox.getSize().y + 60.f);

    attackText.setFont(font);
    attackText.setCharacterSize(18);
    attackText.setFillColor(Color::White);
    attackText.setString("Ataca");
    attackText.setPosition(attackButton.getPosition().x + 90.f, attackButton.getPosition().y + 5.f);
    
    labourButton.setSize(sf::Vector2f(200.f, 40.f));

    labourButton.setPosition(window.getSize().x - labourButton.getSize().x - 50.f, window.getSize().y - 100.f);
    labourButton.setFillColor(sf::Color::Red);
    labourButton.setOutlineColor(sf::Color::Black);
    labourButton.setOutlineThickness(2.f);

    labourText.setFont(font);
    labourText.setCharacterSize(18);
    labourText.setFillColor(sf::Color::White);
    labourText.setString("Labour OFF");
    labourText.setPosition(
        window.getSize().x - labourButton.getSize().x - 50.f + 15.f,
        labourButton.getPosition().y + 8.f
    );

    for (int i = 0; i < 4; i++) {
        Text button;
        button.setFont(font);
        button.setString(names[i]);
        button.setCharacterSize(30);
        button.setFillColor(colors[i]);
        button.setPosition(overlayPanel.getPosition().x - 100.f, overlayPanel.getPosition().y - 80.f + i * 60.f);
        countryButtons.push_back(button);
    }
}
