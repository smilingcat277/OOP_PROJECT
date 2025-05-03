#pragma once
#include <SFML/Graphics.hpp>

void setupLevelText(sf::Text& text, sf::Font& font);
void drawMap1(sf::RenderWindow& window, const int map1[10][16], float TILE_SIZE);
