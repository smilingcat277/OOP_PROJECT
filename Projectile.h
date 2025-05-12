// Projectile.h
#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "GameObject.h"
#include <cmath>

// Forward declaration
class SpriteSheet;

enum class ProjectileType {
    FIREBALL,
    SWORD,
    ARROW,
    MAGIC_BEAM
};

class Projectile : public GameObject {
private:
    float speedX;
    float speedY;
    int damage;
    float lifetime;
    float maxLifetime;
    float maxDistance;
    sf::Vector2f startPosition;
    ProjectileType type;
    float rotationSpeed;
    bool facingRight;

public:
    Projectile(const std::string& texturePath, float x, float y, float speedX, float speedY, 
               ProjectileType type = ProjectileType::SWORD, bool facingRight = true);
    
    // Override methods from GameObject
    void update(float deltaTime) override;
    void draw(sf::RenderWindow& window) override;
    
    // Projectile-specific methods
    void setTextureRect(const sf::IntRect& rect);
    void setMaxDistance(float distance) { maxDistance = distance; }
    
    // Getters and setters
    int getDamage() const { return damage; }
    void setDamage(int newDamage) { damage = newDamage; }
    bool isExpired() const { return lifetime >= maxLifetime || distanceTraveled() >= maxDistance; }
    ProjectileType getType() const { return type; }
    
private:
    float distanceTraveled() const;
    void updateSwordRotation(float deltaTime);
};

#endif // PROJECTILE_H