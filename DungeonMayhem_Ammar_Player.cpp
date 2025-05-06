#include "player.h"
#include <iostream>

void handlePlayerMovement(sf::RectangleShape& player) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){
        player.move(1.f, 0.f);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){
        player.move(-1.f, 0.f);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){
        player.move(0.f, -1.f);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)){
        player.move(0.f, 1.f);
    }
}

void handlePlayerCollision(sf::RectangleShape& player, bool& levelComplete) {
    sf::Vector2f position = player.getPosition();
    player.setPosition(position);

    //std::cout << position.x << " " << position.y << std::endl;
    if (!levelComplete){
        if (position.x < 0.f){
            position.x = 0.f;
        }
        if ((position.x + 50.f > 1600.f) && (position.y >= 500.f && position.y + 50.f <= 600.f)){
            position = player.getPosition();
            levelComplete = true;
        }
        else if ((position.x + 50.f) > 1600.f && position.x + 50.f < 1660){
            position.x = 1550.f;
        }
        if (position.y < 0.f){
            position.y = 0.f;
        }
        if ((position.y + 50.f) > 1000.f){
            position.y = 950.f;
        }
        if (position.x + 50.f > 200.f && position.x < 201.f && position.y >= 0.f && position.y + 50.f < 400.f){
            position.x = 150.f;
        }
        if (position.x + 50.f > 200.f && position.x < 201.f && position.y + 50.f >= 500.f && position.y < 600.f){
          position.x = 150.f;
        }
        if (position.x < 100.f && position.y >= 200.f){
            position.x = 100.f;
        }
        if (position.x < 100.f && position.y + 50.f >= 200.f){
            position.y = 150.f;
        }
        if (position.y + 50.f >= 900.f && position.x >= 100.f && position.x + 50.f <= 400.f){
            position.y = 850.f;
        }
        if (position.x + 50.f >= 400.f && position.x + 50.f <= 401.f  && position.y + 50.f <= 900.f && position.y + 50.f >= 800.f){
            position.x = 350.f;
        }
        if (position.x >= 360.f && position.x <= 780.f && position.y + 50.f >= 800.f){
            position.y = 750.f;
        }
        if (position.y <= 600.f && position.y >= 599.f && position.x >= 200.f && position.x <= 900.f){
            position.y = 600.f;
        }
        if (position.y >= 600.f && position.y <= 780.f && position.x + 50.f >= 900.f && position.x + 50.f <= 901.f){
            position.x = 850.f;
        }
        if (position.x <= 800.f && position.x >= 799.f && position.y >= 800.f && position.y + 50.f <= 900.f){
            position.x = 800.f;
        }
        if (position.x >= 800.f && position.x + 50.f <= 1100.f && position.y + 50.f >= 900.f){
            position.y = 850.f;
        }
        if (position.x >= 900.f && position.x + 50.f <= 1100.f && position.y <= 800.f && position.y >= 799.f){
            position.y = 800.f;
        }
        if (position.y >= 800.f && position.y + 50.f <= 900.f && position.x + 50.f >= 1100.f && position.x + 50.f <= 1101.f){
            position.x = 1050.f;
        }
        if (position.x >= 200.f && position.x + 50.f <= 1100.f && position.y + 50.f >= 500.f && position.y + 50.f <= 501.f){
            position.y = 450.f;
        }
        if (position.y + 50.f <= 500.f && position.y >= 400.f && position.x + 50.f >= 1100.f && position.x + 50.f <= 1101.f){
            position.x = 1050.f;
        }
        if (position.y + 50.f <= 500.f && position.y >= 100.f && position.x <= 400.f && position.x >= 399.f){
            position.x = 400.f;
        }
        if (position.x >= 400.f && position.x + 50.f <= 1500.f && position.y <= 100.f && position.y >= 99.f){
            position.y = 100.f;
        }
        if (position.x >= 450.f && position.x + 50.f <= 1100.f && position.y <= 400.f && position.y >= 399.f){
            position.y = 400.f;
        }
        if (position.y >= 150.f && position.y + 50.f <= 450.f && position.x + 50.f >= 500.f && position.x + 50.f <= 501.f){
            position.x = 450.f;
        }
        if (position.x + 50.f >= 500.f && position.x + 50.f <= 1250.f && position.y + 50.f >= 200.f && position.y <= 151.f){
            position.y = 150.f;
        }
        if (position.y + 50.f >= 200.f && position.y + 50.f <= 800.f && position.x <= 1200.f && position.x >= 1199.f){
            position.x = 1200.f;
        }
        if (position.x >= 1200.f && position.x <= 1400.f && position.y + 50.f >= 800.f && position.y + 50.f <= 801.f){
            position.y = 750.f;
        }
        if (position.y >= 100.f && position.y + 50.f <= 200.f && position.x + 50.f >= 1500.f){
            position.x = 1450.f;
        }
        if (position.x + 50.f >= 1300.f && position.x + 50.f <= 1500.f && position.y + 50.f >= 200.f && position.y + 50.f <= 201.f){
            position.y = 150.f;
        }
        if (position.y + 50.f >= 200.f && position.y <= 700.f && position.x + 50.f >= 1300.f && position.x + 50.f <= 1301.f){
            position.x = 1250.f;
        }
        if (position.x + 50.f >= 1300.f && position.x <= 1400.f && position.y <= 700.f && position.y >= 699.f){
            position.y = 700.f;
        }
        if (position.y + 50.f >= 800.f && position.y + 50.f <= 1000.f && position.x <= 1400.f && position.x >= 1399.f){
            position.x = 1400.f;
        }
        if (position.y >= 500.f && position.y <= 700.f && position.x <= 1400.f && position.x >= 1399.f){
            position.x = 1400.f;
        }
        if (position.y + 50.f >= 600.f && position.y + 50.f <= 1000.f && position.x + 50.f >= 1500.f && position.x + 50.f <= 1501.f){
            position.x = 1450.f;
        }
        if (position.x + 50.f >= 1500.f && position.x + 50.f <= 1600.f && position.y + 50.f >= 600.f && position.y + 50.f <= 601.f){
            position.y = 550.f;
        }

        player.setPosition(position);
    }

    if (levelComplete){
        //std::cout << position.x << " " << position.y << std::endl;
        if (position.x <= 1600.f){
            position.x = 1600.f;
        }
        if (position.x + 50.f >= 3200.f){
            position.x = 3150.f;
        }
        if (position.y + 50.f >= 1000.f){
            position.y = 950.f;
        }
        if (position.y <= 0.f){
            position.y = 0.f;
        }
        player.setPosition(position);
    }
}
