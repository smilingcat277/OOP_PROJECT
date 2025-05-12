// GameObject.h - Base abstract class
#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <stdexcept>
#include <memory>
#include <string>

class GameObject {
protected:
    sf::Sprite sprite;
    sf::Texture texture;
    sf::Vector2f position;
    sf::Vector2f velocity;
    bool isActive;

    // Protected constructor to prevent direct instantiation of abstract class
    GameObject(const std::string& texturePath = "", float x = 0.0f, float y = 0.0f);

    // Protected method to load texture with error handling
    virtual bool loadTexture(const std::string& texturePath);

public:
    virtual ~GameObject() = default;  // Virtual destructor for proper cleanup in derived classes
    
    // Pure virtual methods (must be implemented by derived classes)
    virtual void update(float deltaTime) = 0;
    virtual void draw(sf::RenderWindow& window) = 0;
    
    // Common functionality for all game objects
    virtual bool isColliding(const GameObject& other) const;
    virtual sf::FloatRect getBounds() const;
    
    // Getters and setters with proper encapsulation
    const sf::Vector2f& getPosition() const { return position; }
    void setPosition(const sf::Vector2f& newPosition);
    void setPosition(float x, float y);
    
    bool getIsActive() const { return isActive; }
    void setIsActive(bool active) { isActive = active; }
};

#endif // GAME_OBJECT_H 