// Game.cpp
#include "Game.h"
#include <iostream>
#include <bits/unique_ptr.h>

Game::Game(unsigned int width, unsigned int height, const std::string& title) {
    try {
        window.create(sf::VideoMode(width, height), title);
        window.setFramerateLimit(60);

        // Load projectile texture if you have a separate one
        // if (!projectileTexture.loadFromFile("assets/projectiles.png")) {
        //     throw std::runtime_error("Failed to load projectile texture");
        // }

        player = std::make_unique<Player>();
        initializeLevel();
    }
    catch (const std::exception& e) {
        std::cerr << "Exception in Game constructor: " << e.what() << std::endl;
    }
}

void Game::run() {
    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();

        processEvents();
        update(deltaTime);
        render();
        cleanup();
    }
}

void Game::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
    }

    player->handleInput();

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        player->attack();
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::K)) {
        player->castSpell(projectiles);
    }
}

void Game::update(float deltaTime) {
    player->update(deltaTime);
    player->handleCollisions(tiles);

    for (auto& projectile : projectiles) {
        if (projectile->getIsActive()) {
            projectile->update(deltaTime);
        }
    }
}

void Game::render() {
    window.clear(sf::Color(50, 50, 70));

    // Draw level elements
    for (const auto& tile : tiles) {
        window.draw(tile);
    }

    // Draw player
    player->draw(window);

    // Draw projectiles
    for (const auto& projectile : projectiles) {
        if (projectile->getIsActive()) {
            projectile->draw(window);
        }
    }

    // UI elements would be drawn here

    window.display();
}

void Game::cleanup() {
    // Remove inactive projectiles
    projectiles.erase(
        std::remove_if(
            projectiles.begin(),
            projectiles.end(),
            [](const auto& p) { return !p->getIsActive(); }
        ),
        projectiles.end()
    );
}

void Game::initializeLevel() {
    // Create some sample tiles
    sf::RectangleShape floor;
    floor.setSize(sf::Vector2f(800, 20));
    floor.setPosition(0, 500);
    floor.setFillColor(sf::Color(100, 100, 100));

    tiles.push_back(floor);

    // Add more level elements as needed
}