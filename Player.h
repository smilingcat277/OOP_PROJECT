// Player.h
#ifndef PLAYER_H
#define PLAYER_H

#include "Character.h"
#include "Projectile.h"
#include <memory>
#include <vector>

// Forward declaration
class Projectile;

enum class PlayerState {
    IDLE,
    WALKING,
    ATTACKING,
    CASTING,
    HURT
};

class Player : public Character {
private:
    int mana;
    int maxMana;
    float jumpStrength;
    bool isOnGround;
    
    sf::Clock attackCooldownClock;
    sf::Clock magicCooldownClock;
    sf::Clock manaCooldownClock;
    
    PlayerState currentState;
    
    // Animation related members
    int currentFrame;
    float frameTime;
    float animationTimer;
    
    void updateAnimation(float deltaTime);
    void regenerateMana(float deltaTime);

public:
    // Constructors with default parameters for better flexibility
    Player();
    Player(const std::string& texturePath, float x, float y);
    
    // Override methods from base classes
    void handleMovement(float deltaTime) override;
    void update(float deltaTime) override;
    void draw(sf::RenderWindow& window) override;
    
    // Player-specific methods
    void handleInput();
    void attack();
    void castSpell(std::vector<std::shared_ptr<Projectile>>& projectiles);
    
    // Collision handling
    void handleCollisions(const std::vector<sf::RectangleShape>& collidables);
    
    // Getters and setters
    int getMana() const { return mana; }
    int getMaxMana() const { return maxMana; }
    void setMana(int newMana);
};

#endif // PLAYER_H