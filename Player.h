#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include <vector>

class Player {
private:
    sf::Sprite sprite;
    sf::Texture texture;
    sf::Vector2f velocity;
    float speed;
    float jumpStrength;
    bool isOnGround;

    int health;
    int maxHealth;

    int mana;
    int maxMana;

    sf::Clock attackCooldownClock;
    sf::Clock magicCooldownClock;

    bool facingRight;

    void loadTexture();
    void updateAnimation();

public:
    Player();

    void handleInput();
    void update(float deltaTime, const std::vector<sf::RectangleShape>& collidables);
    void draw(sf::RenderWindow& window);

    void attack(); // melee
    void castSpell(std::vector<sf::Sprite>& projectiles); // magic
    void takeDamage(int damage);
    void regenerateMana();
    sf::FloatRect getBounds() const;

    // Getters
    int getHealth() const;
    int getMana() const;
};

#endif
