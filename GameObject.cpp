// GameObject.cpp
#include "GameObject.h"

GameObject::GameObject(const std::string& texturePath, float x, float y) : 
    position(x, y), velocity(0.0f, 0.0f), isActive(true) {
    if (!texturePath.empty()) {
        loadTexture(texturePath);
    }
    sprite.setPosition(position);
}

bool GameObject::loadTexture(const std::string& texturePath) {
    try {
        if (!texture.loadFromFile(texturePath)) {
            std::cerr << "Failed to load texture: " << texturePath << std::endl;
            return false;
        }
        sprite.setTexture(texture);
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Exception occurred when loading texture: " << e.what() << std::endl;
        return false;
    }
}

bool GameObject::isColliding(const GameObject& other) const {
    return getBounds().intersects(other.getBounds());
}

sf::FloatRect GameObject::getBounds() const {
    return sprite.getGlobalBounds();
}

void GameObject::setPosition(const sf::Vector2f& newPosition) {
    position = newPosition;
    sprite.setPosition(position);
}

void GameObject::setPosition(float x, float y) {
    position.x = x;
    position.y = y;
    sprite.setPosition(position);
}