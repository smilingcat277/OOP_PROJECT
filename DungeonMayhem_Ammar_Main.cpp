#include <SFML/Graphics.hpp>
#include <iostream>
#include "map1.h"
#include "player.h"
#include "level.h"

int main(){

    bool level1Complete = false;
    bool level2Complete = false;
    bool transitioningCenter = false;
    sf::Vector2f targetCenter;
    bool transitioningView = false;
    sf::Vector2f targetViewSize;
    float viewTransitionSpeed = 0.5f;

    sf::RenderWindow window(sf::VideoMode(1600, 1000), "SFML works!");
    const float TILE_SIZE = 100.f;

    sf::Font font;
    sf::Text text1;
    setupLevelText(text1, font);

    sf::RectangleShape player(sf::Vector2f(50.f, 50.f));
    player.setPosition(sf::Vector2f(0.f, 0.f));
    player.setFillColor(sf::Color::White);

    sf::RectangleShape door(sf::Vector2f(100.f, 100.f));
    door.setPosition(sf::Vector2f(1900.f, -1050.f));
    door.setFillColor(sf::Color::Red);

    sf::RectangleShape map2(sf::Vector2f(700.f, 2000.f));
    map2.setPosition(sf::Vector2f(1600.f, -1000.f));
    map2.setFillColor(sf::Color(0xADD8E6));

    sf::RectangleShape map3(sf::Vector2f(1600.f, 1000.f));
    map3.setPosition(sf::Vector2f(1150.f, -2000.f));
    map3.setFillColor(sf::Color(255, 192, 203));

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

        if (!transitioningView && !level2Complete){
            camera.setCenter(player.getPosition());
        }

        window.setView(camera);

        if (level2Complete && !transitioningView){
            transitioningView = true;
            targetViewSize = sf::Vector2f(1650.f, 1050.f);
        }

        if (transitioningView){

            sf::Vector2f currentSize = camera.getSize();

            currentSize += (targetViewSize - currentSize) * 0.05f * viewTransitionSpeed;

            camera.setSize(currentSize);

            if (std::abs(currentSize.x - targetViewSize.x) < 1.f && std::abs(currentSize.y - targetViewSize.y) < 1.f){
                camera.setSize(targetViewSize);
                transitioningView = false;

                transitioningCenter = true;
                targetCenter = sf::Vector2f(1950.f, -1475.f); // center of map3
            }
        }

        if (transitioningCenter) {
            sf::Vector2f currentCenter = camera.getCenter();
            currentCenter += (targetCenter - currentCenter) * 0.05f * viewTransitionSpeed;
            camera.setCenter(currentCenter);

            if (std::abs(currentCenter.x - targetCenter.x) < 1.f && std::abs(currentCenter.y - targetCenter.y) < 1.f){
                camera.setCenter(targetCenter);
                transitioningCenter = false;
            }
        }

        handlePlayerMovement(player);

        handlePlayerCollision(player, level1Complete, level2Complete);

        window.clear();

        drawMap1(window, map1, TILE_SIZE);

        window.draw(map2);
        window.draw(map3);
        window.draw(door);
        window.draw(text1);
        window.draw(player);
        window.display();
    }

    return 0;
}
