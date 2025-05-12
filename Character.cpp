// Character.cpp
#include "Character.h"

Character::Character(const std::string& texturePath, float x, float y, float speed, int maxHealth) :
    GameObject(texturePath, x, y),
    speed(speed),
    health(maxHealth),
    maxHealth(maxHealth),
    facingRight(true) {
}

void Character::update(float deltaTime) {
    handleMovement(deltaTime);
    position += velocity * deltaTime;
    sprite.setPosition(position);
}

void Character::takeDamage(int damage) {
    if (damage < 0) {
        throw std::invalid_argument("Damage cannot be negative");
    }
    health = std::max(0, health - damage);
}

void Character::heal(int amount) {
    if (amount < 0) {
        throw std::invalid_argument("Healing amount cannot be negative");
    }
    health = std::min(maxHealth, health + amount);
}

void Character::move(float dx, float dy) {
    velocity.x = dx;
    velocity.y = dy;
    
    if (dx > 0) facingRight = true;
    else if (dx < 0) facingRight = false;
}

void Character::move(const sf::Vector2f& direction) {
    move(direction.x, direction.y);
}