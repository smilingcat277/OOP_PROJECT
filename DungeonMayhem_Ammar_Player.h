#pragma once
#include <SFML/Graphics.hpp>

void handlePlayerMovement(sf::RectangleShape& player);
void handlePlayerCollision(sf::RectangleShape& player, bool& levelComplete, bool& leve2Complete);
