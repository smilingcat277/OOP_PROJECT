// Projectile.cpp
#include "Projectile.h"

Projectile::Projectile(const std::string& texturePath, float x, float y, float speedX, float speedY, ProjectileType type, bool facingRight) :
    GameObject(texturePath, x, y),
    speedX(speedX),
    speedY(speedY),
    damage(10),
    lifetime(0.0f),
    maxLifetime(2.0f),
    maxDistance(400.0f),
    startPosition(x, y),
    type(type),
    rotationSpeed(720.0f),  // 720 degrees per second
    facingRight(facingRight) {
    
    // Adjust damage based on projectile type
    switch (type) {
        case ProjectileType::SWORD:
            damage = 15;
            maxDistance = 300.0f;
            break;
        case ProjectileType::FIREBALL:
            damage = 12;
            maxDistance = 500.0f;
            break;
        case ProjectileType::ARROW:
            damage = 8;
            maxDistance = 600.0f;
            break;
        case ProjectileType::MAGIC_BEAM:
            damage = 20;
            maxDistance = 350.0f;
            break;
    }
    
    // Set origin to center for rotation (assuming 64x64 texture)
    sprite.setOrigin(32.0f, 32.0f);
    
    // Initial rotation based on direction
    if (!facingRight) {
        sprite.setRotation(180.0f);
    }
}

void Projectile::update(float deltaTime) {
    position.x += speedX * deltaTime;
    position.y += speedY * deltaTime;
    sprite.setPosition(position);
    
    lifetime += deltaTime;
    
    // Check if the projectile has exceeded its max distance or lifetime
    if (lifetime >= maxLifetime || distanceTraveled() >= maxDistance) {
        isActive = false;
    }
    
    // Special update for sword projectiles
    if (type == ProjectileType::SWORD) {
        updateSwordRotation(deltaTime);
    }
}

void Projectile::draw(sf::RenderWindow& window) {
    if (isActive) {
        window.draw(sprite);
    }
}

void Projectile::setTextureRect(const sf::IntRect& rect) {
    sprite.setTextureRect(rect);
}

float Projectile::distanceTraveled() const {
    float dx = position.x - startPosition.x;
    float dy = position.y - startPosition.y;
    return std::sqrt(dx*dx + dy*dy);
}

void Projectile::updateSwordRotation(float deltaTime) {
    // Rotate the sword as it flies
    float currentRotation = sprite.getRotation();
    float rotationAmount = rotationSpeed * deltaTime;
    
    if (facingRight) {
        sprite.setRotation(currentRotation + rotationAmount);
    } else {
        sprite.setRotation(currentRotation - rotationAmount);
    }
}