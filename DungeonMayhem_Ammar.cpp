#include <SFML/Graphics.hpp>
#include<iostream>

int main()
{   
    sf::RenderWindow window(sf::VideoMode(1500, 1000), "SFML works!");
    const float TILE_SIZE = 100.f;

    sf::Font font;
    sf::Text text1;
    if (!font.loadFromFile("CONSOLAB.TTF")){
        return -1;
    }

    text1.setFont(font);
    text1.setString("Level 1");
    text1.setCharacterSize(100.f);
    text1.setFillColor(sf::Color::Black);
    text1.setPosition(sf::Vector2f(615.f, 100.f));

    sf::Text text2;
    if (!font.loadFromFile("CONSOLAB.TTF")){
        return -1;
    }

    text2.setFont(font);
    text2.setString("Level 2");
    text2.setCharacterSize(100.f);
    text2.setFillColor(sf::Color::White);
    //text2.setPosition(sf::Vector2f(615.f, 100.f));

    sf::RectangleShape player(sf::Vector2f(50.f, 50.f));
    player.setPosition(sf::Vector2f(0.f, 0.f));
    player.setFillColor(sf::Color::White);

    sf::RectangleShape rectangle2(sf::Vector2f(1600.f, 1000.f));
    rectangle2.setPosition(sf::Vector2f(1600.f, 0.f));
    rectangle2.setFillColor(sf::Color(100, 100, 100));

    bool levelComplete = false;

    int maze[10][16] = {
        {0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1},
        {1, 0, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1},
        {1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1},
        {1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 0},
        {0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1},
        {1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1},
        {1, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1},
    };

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
        
        sf::Vector2f position = player.getPosition();
        sf::Vector2f nextPosition = position;
        player.setPosition(position);

        for (int row = 0; row < 10; ++row) {
            for (int col = 0; col < 16; ++col) {
                sf::RectangleShape tile(sf::Vector2f(TILE_SIZE, TILE_SIZE));
                tile.setPosition(col * TILE_SIZE, row * TILE_SIZE);

                if (maze[row][col] == 1)
                    tile.setFillColor(sf::Color::Yellow);
                else
                    tile.setFillColor(sf::Color(100, 100, 100));

                window.draw(tile);
            }
        }

        //std::cout << centerX << ", " << centerY << std::endl;

        if (nextPosition.x < 0.f){
            nextPosition.x = 0.f;
        }
        if ((nextPosition.x + 50.f > 1600.f) && (nextPosition.y >= 500.f && nextPosition.y + 50.f <= 600.f)){
            nextPosition = player.getPosition();
        }
        else if ((nextPosition.x + 50.f) > 1600.f && nextPosition.x + 50.f < 1660){
            nextPosition.x = 1550.f;
        }
        if (nextPosition.y < 0.f){
            nextPosition.y = 0.f;
        }
        if ((nextPosition.y + 50.f) > 1000.f){
            nextPosition.y = 950.f;
        }
        
        player.setPosition(nextPosition);
        
        // rectangle1.setPosition(position);
        window.draw(rectangle2);
        window.draw(text1);
        window.draw(player);
        window.display();
        window.clear();
    }
    return 0;
}
