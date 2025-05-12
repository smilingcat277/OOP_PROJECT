// Game.h - Game manager class
#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "Player.h"
#include "Projectile.h"

class Game {
private:
    sf::RenderWindow window;
    sf::Clock clock;

    std::unique_ptr<Player> player;
    std::vector<sf::RectangleShape> tiles;
    std::vector<std::shared_ptr<Projectile>> projectiles;

    void processEvents();
    void update(float deltaTime);
    void render();
    void cleanup();

public:
    Game(unsigned int width = 800, unsigned int height = 600, const std::string& title = "Dungeon Mayhem");
    ~Game() = default;

    void run();
    void initializeLevel();
};

#endif // GAME_H