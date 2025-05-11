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

void drawMap1(sf::RenderWindow& window, const int map1[10][16], float TILE_SIZE, sf::Texture &brickTexture, sf::Texture &GroundTexture) {
    for (int row = 0; row < 10; ++row) {
        for (int col = 0; col < 16; ++col) {
            sf::RectangleShape tile(sf::Vector2f(TILE_SIZE, TILE_SIZE));
            tile.setPosition(col * TILE_SIZE, row * TILE_SIZE);

            if (map1[row][col] == 1)
                tile.setTexture(&brickTexture);
            else
                tile.setTexture(&GroundTexture);

            window.draw(tile);
        }
    }
}

void drawMap2(sf::RenderWindow& window, const int map2[20][9], float TILE_SIZE, sf::Texture &brickTexture ,sf::Texture &GroundTexture, sf::Vector2f offset){
    for (int row = 0; row < 20; ++row){
        for (int col = 0; col < 9; ++col){
            sf::RectangleShape tile(sf::Vector2f(TILE_SIZE, TILE_SIZE));
            tile.setPosition(col * TILE_SIZE + offset.x, row * TILE_SIZE + offset.y);

            if (map2[row][col] == 1)
                tile.setTexture(&brickTexture);
            else
                tile.setTexture(&GroundTexture);

            window.draw(tile);
        }
    }
}

void drawMap3(sf::RenderWindow& window, const int map3[12][17], float TILE_SIZE, sf::Texture &brickTexture, sf::Texture &GroundTexture, sf::Vector2f offset){
    for (int row = 0; row < 12; ++row){
        for (int col = 0; col < 17; ++col){
            sf::RectangleShape tile(sf::Vector2f(TILE_SIZE, TILE_SIZE));
            tile.setPosition(col * TILE_SIZE + offset.x, row * TILE_SIZE + offset.y);

            if (map3[row][col] == 1)
                tile.setTexture(&brickTexture);
            else
                tile.setTexture(&GroundTexture);

            window.draw(tile);
        }
    }
}
