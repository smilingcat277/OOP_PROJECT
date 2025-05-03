#include <SFML/Graphics.hpp>
#include <iostream>
#include "map1.h"
#include "player.h"
#include "level.h"

int main()
{   
    sf::RenderWindow window(sf::VideoMode(1500, 1000), "SFML works!");
    const float TILE_SIZE = 100.f;

    sf::Font font;
    sf::Text text1;
    setupLevelText(text1, font);

    sf::RectangleShape player(sf::Vector2f(50.f, 50.f));
    player.setPosition(sf::Vector2f(0.f, 0.f));
    player.setFillColor(sf::Color::White);

    sf::RectangleShape rectangle2(sf::Vector2f(1600.f, 1000.f));
    rectangle2.setPosition(sf::Vector2f(1600.f, 0.f));
    rectangle2.setFillColor(sf::Color(100, 100, 100));

    bool levelComplete = false;

    sf::View camera;
    camera.setSize(sf::Vector2f(1000.f, 900.f));
    
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        camera.setCenter(player.getPosition());
        window.setView(camera);

        handlePlayerMovement(player);
        handlePlayerCollision(player, levelComplete);

        window.clear();

        drawMap1(window, map1, TILE_SIZE);

        window.draw(rectangle2);
        window.draw(text1);
        window.draw(player);
        window.display();
    }
    return 0;
}
