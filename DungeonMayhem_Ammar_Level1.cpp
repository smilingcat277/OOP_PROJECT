#include "level.h"

void setupLevelText(sf::Text& text, sf::Font& font) {
    if (!font.loadFromFile("CONSOLAB.TTF")){
        exit(-1);
    }
    text.setFont(font);
    text.setString("Level 1");
    text.setCharacterSize(100.f);
    text.setFillColor(sf::Color::Black);
    text.setPosition(sf::Vector2f(615.f, 100.f));
}

void drawMap1(sf::RenderWindow& window, const int map1[10][16], float TILE_SIZE) {
    for (int row = 0; row < 10; ++row) {
        for (int col = 0; col < 16; ++col) {
            sf::RectangleShape tile(sf::Vector2f(TILE_SIZE, TILE_SIZE));
            tile.setPosition(col * TILE_SIZE, row * TILE_SIZE);

            if (map1[row][col] == 1)
                tile.setFillColor(sf::Color::Yellow);
            else
                tile.setFillColor(sf::Color(100, 100, 100));

            window.draw(tile);
        }
    }
}
