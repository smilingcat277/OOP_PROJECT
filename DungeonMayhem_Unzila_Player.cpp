#include "Player.h"

Player::Player() {
    loadTexture();
    sprite.setTexture(texture);
    sprite.setTextureRect(sf::IntRect(0, 0, 64, 64));
    sprite.setPosition(100, 100);

    speed = 200.0f;
    jumpStrength = 300.0f;
    velocity = sf::Vector2f(0, 0);
    isOnGround = false;
    health = maxHealth = 100;
    mana = maxMana = 100;
    facingRight = true;
}

void Player::loadTexture() {
    texture.loadFromFile("assets/player.png");
}

void Player::handleInput() {
    velocity.x = 0;
    velocity.y = 0;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        velocity.x -= speed;
        facingRight = false;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        velocity.x += speed;
        facingRight = true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        velocity.y -= speed;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        velocity.y += speed;
    }
}

void Player::update(float dt, const std::vector<sf::RectangleShape>& collidables) {
    sprite.move(velocity * dt);
    regenerateMana();
}

void Player::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

void Player::attack() {
    if (attackCooldownClock.getElapsedTime().asSeconds() > 0.5f) {
        attackCooldownClock.restart();
    }
}

void Player::castSpell(std::vector<sf::Sprite>& projectiles) {
    if (magicCooldownClock.getElapsedTime().asSeconds() > 1.0f && mana >= 10) {
        sf::Sprite projectile;
        projectile.setTexture(texture);
        projectile.setTextureRect(sf::IntRect(64, 0, 64, 64));
        projectile.setPosition(sprite.getPosition().x + (facingRight ? 60 : -10), sprite.getPosition().y);
        projectiles.push_back(projectile);
        mana -= 10;
        magicCooldownClock.restart();
    }
}

void Player::takeDamage(int damage) {
    health -= damage;
    if (health < 0) health = 0;
}

void Player::regenerateMana() {
    if (mana < maxMana)
        mana += 1;
}

sf::FloatRect Player::getBounds() const {
    return sprite.getGlobalBounds();
}

int Player::getHealth() const { return health; }
int Player::getMana() const { return mana; }
