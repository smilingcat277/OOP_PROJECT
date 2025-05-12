// Character.h - Abstract base class for all characters 
#ifndef CHARACTER_H
#define CHARACTER_H

#include "GameObject.h"
#include <vector>

class Character : public GameObject {
protected:
    float speed;
    int health;
    int maxHealth;
    bool facingRight;

    // Protected constructor to prevent direct instantiation
    Character(const std::string& texturePath, float x, float y, float speed, int maxHealth);

public:
    virtual ~Character() = default;
    
    // Pure virtual methods
    virtual void handleMovement(float deltaTime) = 0;
    
    // Common character functionality
    virtual void update(float deltaTime) override;
    virtual void takeDamage(int damage);
    virtual void heal(int amount);
    
    // Method overloading for different movement patterns
    virtual void move(float dx, float dy);
    virtual void move(const sf::Vector2f& direction);
    
    // Getters and setters
    int getHealth() const { return health; }
    int getMaxHealth() const { return maxHealth; }
    float getSpeed() const { return speed; }
    void setSpeed(float newSpeed) { speed = newSpeed; }
    bool isFacingRight() const { return facingRight; }
    bool isAlive() const { return health > 0; }
};

#endif // CHARACTER_H