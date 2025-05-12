// Player.cpp
#include "Player.h"
#include <iostream>

Player::Player() : 
    Player("assets/player.png", 100.0f, 100.0f) {
}

Player::Player(const std::string& texturePath, float x, float y) :
    Character(texturePath, x, y, 200.0f, 100),
    mana(100),
    maxMana(100),
    jumpStrength(300.0f),
    isOnGround(false),
    currentState(PlayerState::IDLE),
    currentFrame(0),
    frameTime(0.1f),
    animationTimer(0.0f) {
    
    try {
        if (!loadTexture(texturePath)) {
            // If loading fails, use a default texture or indicate error
            std::cerr << "Failed to load player texture, using fallback" << std::endl;
            sprite.setTextureRect(sf::IntRect(0, 0, 64, 64));
            sprite.setColor(sf::Color::Red); // Indicate error visually
        } else {
            sprite.setTextureRect(sf::IntRect(0, 0, 64, 64));
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Exception in Player constructor: " << e.what() << std::endl;
        sprite.setTextureRect(sf::IntRect(0, 0, 64, 64));
        sprite.setColor(sf::Color::Red);
    }
}

void Player::handleInput() {
    velocity.x = 0;
    velocity.y = 0;
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        velocity.x -= speed;
        facingRight = false;
        currentState = PlayerState::WALKING;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        velocity.x += speed;
        facingRight = true;
        currentState = PlayerState::WALKING;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        velocity.y -= speed;
        currentState = PlayerState::WALKING;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        velocity.y += speed;
        currentState = PlayerState::WALKING;
    }
    
    if (velocity.x == 0 && velocity.y == 0) {
        currentState = PlayerState::IDLE;
    }
}

void Player::update(float deltaTime) {
    Character::update(deltaTime);
    regenerateMana(deltaTime);
    updateAnimation(deltaTime);
}

void Player::draw(sf::RenderWindow& window) {
    if (isActive) {
        window.draw(sprite);
    }
}

void Player::handleMovement(float deltaTime) {
    // Already handled in handleInput(), nothing additional needed here
}

void Player::attack() {
    if (attackCooldownClock.getElapsedTime().asSeconds() > 0.5f) {
        currentState = PlayerState::ATTACKING;
        attackCooldownClock.restart();
        // Attack logic would go here
    }
}

void Player::castSpell(std::vector<std::shared_ptr<Projectile>>& projectiles) {
    if (magicCooldownClock.getElapsedTime().asSeconds() > 0.5f && mana >= 10) {
        currentState = PlayerState::CASTING;
        
        try {
            // Create a sword projectile
            auto projectile = std::make_shared<Projectile>(
                "assets/weapon.png",                  // Base texture
                position.x + (facingRight ? 60 : -10), // Spawn position X
                position.y + 0,                        // Spawn position Y (adjusted to center)
                facingRight ? 600.0f : -600.0f,        // Speed in x direction (doubled for faster projectile)
                0.0f,                                  // Speed in y direction
                ProjectileType::SWORD,                 // Projectile type
                facingRight                            // Direction
            );
            
            // Set the texture rect for the sword projectile (using a different part of spritesheet)
            // This assumes the sword texture is at position (128, 0) with size 64x64
            projectile->setTextureRect(sf::IntRect(128, 0, 16, 16));
            
            // Set projectile properties
            projectile->setDamage(15);
            
            projectiles.push_back(projectile);
            
            mana -= 10;
            magicCooldownClock.restart();
        }
        catch (const std::exception& e) {
            std::cerr << "Exception when casting spell: " << e.what() << std::endl;
        }
    }
}

void Player::handleCollisions(const std::vector<sf::RectangleShape>& collidables) {
    isOnGround = false;
    
    for (const auto& collidable : collidables) {
        sf::FloatRect playerBounds = getBounds();
        sf::FloatRect tileBounds = collidable.getGlobalBounds();
        
        if (playerBounds.intersects(tileBounds)) {
            // Handle collision
            // This is a simplified collision response
            sf::FloatRect intersection;
            playerBounds.intersects(tileBounds, intersection);
            
            if (intersection.width < intersection.height) {
                // Horizontal collision
                if (playerBounds.left < tileBounds.left) {
                    position.x -= intersection.width;
                } else {
                    position.x += intersection.width;
                }
                velocity.x = 0;
            } else {
                // Vertical collision
                if (playerBounds.top < tileBounds.top) {
                    position.y -= intersection.height;
                    velocity.y = 0;
                } else {
                    position.y += intersection.height;
                    velocity.y = 0;
                    isOnGround = true;
                }
            }
            
            sprite.setPosition(position);
        }
    }
}

void Player::updateAnimation(float deltaTime) {
    animationTimer += deltaTime;
    
    if (animationTimer >= frameTime) {
        animationTimer -= frameTime;
        currentFrame = (currentFrame + 1) % 4;  // Assuming 4 frames per animation
        
        int yOffset = 0;
        switch (currentState) {
            case PlayerState::IDLE:
                yOffset = 0;
                break;
            case PlayerState::WALKING:
                yOffset = 64;
                break;
            case PlayerState::ATTACKING:
                yOffset = 128;
                break;
            case PlayerState::CASTING:
                yOffset = 192;
                break;
            case PlayerState::HURT:
                yOffset = 256;
                break;
        }
        
        sprite.setTextureRect(sf::IntRect(currentFrame * 64, yOffset, 64, 64));
    }
    
    // Update sprite direction immediately for swift direction change
    // This is outside the animation timer to respond immediately
    if (facingRight) {
        sprite.setScale(1.0f, 1.0f);
        sprite.setOrigin(0.0f, 0.0f);  // Reset origin when facing right
    } else {
        sprite.setScale(-1.0f, 1.0f);
        sprite.setOrigin(64.0f, 0.0f);  // Adjust origin for flipping
    }
}

void Player::regenerateMana(float deltaTime) {
    if (mana < maxMana && manaCooldownClock.getElapsedTime().asSeconds() > 0.1f) {
        mana = std::min(maxMana, mana + 1);
        manaCooldownClock.restart();
    }
}

void Player::setMana(int newMana) {
    if (newMana < 0) {
        throw std::invalid_argument("Mana cannot be negative");
    }
    mana = std::min(maxMana, newMana);
}