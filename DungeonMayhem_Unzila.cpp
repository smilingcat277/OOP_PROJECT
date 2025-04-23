#include <SFML/Graphics.hpp>
#include "Player.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Dungeon Mayhem");
    window.setFramerateLimit(60);

    Player player;
    std::vector<sf::RectangleShape> tiles;

    sf::Clock clock;
    std::vector<sf::Sprite> projectiles;

    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        player.handleInput();
        player.update(dt, tiles);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::K))
            player.castSpell(projectiles);

        for (auto& p : projectiles)
            p.move(300.f * dt, 0);

        window.clear();
        player.draw(window);
        for (auto& p : projectiles) window.draw(p);
        window.display();
    }

    return 0;
}
