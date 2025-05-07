#include "../headers/init.h"
#include <cmath>

using namespace sf;
using namespace std;

string names[4] = {"Romania", "Bulgaria", "Serbia", "Ungaria"};
Color colors[4] = {
    Color(178, 34, 34),
    Color(0, 139, 139),
    Color(72, 61, 139),
    Color(218, 165, 32)
};

void initializare(RenderWindow& window, vector<Country>& countries) {
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
            tmpRegions[countryId].push_back(Region(i * cols + j, res, hex, false));
        }
    }

    for (int i = 0; i < 4; i++) {
        countries.push_back(Country(i, names[i], tmpRegions[i]));
    }
}
