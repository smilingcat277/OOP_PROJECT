#pragma once
#include <SFML/Graphics.hpp>

void setupLevelText(sf::Text& text, sf::Font& font);
void drawMap1(sf::RenderWindow& window, const int map1[10][16], float TILE_SIZE, sf::Texture &brickTexture, sf::Texture &textureGround);
void drawMap2(sf::RenderWindow& window, const int map2[20][9], float TILE_SIZE, sf::Texture &brickTexture ,sf::Texture &GroundTexture, sf::Vector2f offset);
void drawMap3(sf::RenderWindow& window, const int map3[12][17], float TILE_SIZE, sf::Texture &brickTexture ,sf::Texture &GroundTexture, sf::Vector2f offset);
