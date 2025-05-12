#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include "maps.h"
#include "player.h"
#include "level.h"

// Structure to hold the game state
struct GameState {
    sf::Vector2f playerPosition;
    bool level1Complete;
    bool level2Complete;
    sf::View camera;
};

// Function to save the game state
void saveGame(const GameState& state, const std::string& filename = "savegame.dat") {
    std::ofstream file(filename, std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(&state), sizeof(GameState));
        file.close();
        std::cout << "Game saved to " << filename << std::endl;
    } else {
        std::cerr << "Unable to open file for saving." << std::endl;
    }
}

// Function to load the game state
bool loadGame(GameState& state, const std::string& filename = "savegame.dat") {
    std::ifstream file(filename, std::ios::binary);
    if (file.is_open()) {
        file.read(reinterpret_cast<char*>(&state), sizeof(GameState));
        file.close();
        std::cout << "Game loaded from " << filename << std::endl;
        return true;
    } else {
        std::cerr << "No save file found or unable to open." << std::endl;
        return false;
    }
}

int main() {
    GameState gameState;
    gameState.level1Complete = false;
    gameState.level2Complete = false;
    gameState.camera.setSize(sf::Vector2f(1000.f, 900.f));
    gameState.playerPosition = sf::Vector2f(0.f, 0.f);

    bool transitioningCenter = false;
    sf::Vector2f targetCenter;
    bool transitioningView = false;
    sf::Vector2f targetViewSize;
    float viewTransitionSpeed = 0.5f;

    sf::Texture textureGround;
    if (!textureGround.loadFromFile("2010-09-04_08-50-38.jpg", sf::Rect(0, 0, 100, 100))) {
        std::cout << "Unable to open ground texture\n";
        return -1;
    }

    sf::Texture textureWall;
    if (!textureWall.loadFromFile("plainwallside.jpg", sf::IntRect(18, 18, 100, 100))) {
        std::cout << "Unable to open wall texture\n";
        return -1;
    }

    sf::RenderWindow window(sf::VideoMode(1600, 1000), "SFML works!");
    const float TILE_SIZE = 100.f;

    sf::Font font;
    sf::Text text1;
    setupLevelText(text1, font);

    sf::RectangleShape player(sf::Vector2f(50.f, 50.f));
    sf::RectangleShape door(sf::Vector2f(100.f, 100.f));
    door.setPosition(sf::Vector2f(1900.f, -1050.f));
    door.setFillColor(sf::Color::Red);

    sf::Vector2f map2Position(1500.f, -1000.f);
    sf::Vector2f map3Position(1100.f, -2200.f);

    sf::View camera = gameState.camera;
    player.setPosition(gameState.playerPosition);
    bool level1Complete = gameState.level1Complete;
    bool level2Complete = gameState.level2Complete;

    bool isPaused = false;
    sf::Text pauseText;
    pauseText.setFont(font);
    pauseText.setString("Paused");
    pauseText.setCharacterSize(150.f);
    pauseText.setFillColor(sf::Color::Black);
    pauseText.setPosition(window.getSize().x / 2.f - pauseText.getLocalBounds().width / 2.f,
                          window.getSize().y / 2.f - pauseText.getLocalBounds().height / 2.f);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::P) {
                    isPaused = !isPaused;
                    std::cout << "Game Paused: " << isPaused << std::endl;
                } else if (event.key.code == sf::Keyboard::Escape) {
                    std::cout << "Exiting game." << std::endl;
                    window.close();
                } else if (event.key.code == sf::Keyboard::S) {
                    gameState.playerPosition = player.getPosition();
                    gameState.level1Complete = level1Complete;
                    gameState.level2Complete = level2Complete;
                    gameState.camera = camera;
                    saveGame(gameState);
                } else if (event.key.code == sf::Keyboard::L) {
                    if (loadGame(gameState)) {
                        player.setPosition(gameState.playerPosition);
                        level1Complete = gameState.level1Complete;
                        level2Complete = gameState.level2Complete;
                        camera = gameState.camera;
                        window.setView(camera); // Ensure the loaded view is set
                    }
                }
            }
        }

        if (!isPaused) {
            if (!transitioningView && !level2Complete) {
                camera.setCenter(player.getPosition());
            }

            window.setView(camera);

            if (level2Complete && !transitioningView) {
                transitioningView = true;
                targetViewSize = sf::Vector2f(1650.f, 1200.f);
            }

            if (transitioningView) {
                sf::Vector2f currentSize = camera.getSize();
                currentSize += (targetViewSize - currentSize) * 0.05f * viewTransitionSpeed;
                camera.setSize(currentSize);

                if (std::abs(currentSize.x - targetViewSize.x) < 1.f && std::abs(currentSize.y - targetViewSize.y) < 1.f) {
                    camera.setSize(targetViewSize);
                    transitioningView = false;
                    transitioningCenter = true;
                    targetCenter = sf::Vector2f(1950.f, -1600.f); // center of map3
                }
            }

            if (transitioningCenter) {
                sf::Vector2f currentCenter = camera.getCenter();
                currentCenter += (targetCenter - currentCenter) * 0.05f * viewTransitionSpeed;
                camera.setCenter(currentCenter);

                if (std::abs(currentCenter.x - targetCenter.x) < 1.f && std::abs(currentCenter.y - targetCenter.y) < 1.f) {
                    camera.setCenter(targetCenter);
                    transitioningCenter = false;
                }
            }

            handlePlayerMovement(player);
            handlePlayerCollision(player, level1Complete, level2Complete);
        }

        window.clear();

        drawMap1(window, map1, TILE_SIZE, textureWall, textureGround);
        drawMap2(window, map2, TILE_SIZE, textureWall, textureGround, map2Position);
        drawMap3(window, map3, TILE_SIZE, textureWall, textureGround, map3Position);

        window.draw(text1);
        window.draw(player);

        if (isPaused) {
            window.draw(pauseText);
        }

        window.display();
    }

    return 0;
}
