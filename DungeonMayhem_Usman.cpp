#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cmath>

class Animation {
public:
    std::vector<int> frames;
    std::string name;
    Animation(const std::string& animationName, const std::vector<int>& frameIndices) : name(animationName), frames(frameIndices) {}
    int getFrame(int index) const { return frames[index]; }
    int getNumFrames() const { return frames.size(); }
    const std::string& getName() const { return name; }
};

class AnimatedSprite {
private:
    sf::Texture texture;
    int spriteWidth;
    int spriteHeight;
    int numColumns;
    int numRows;
    std::vector<Animation> animations;
    int currentAnimation;
    int currentFrame;
    float frameTime;
    float elapsedSeconds;
    sf::Clock clock;
    bool isAttacking;
protected:
    sf::Sprite sprite;
public:
    sf::Vector2f position;
    AnimatedSprite(const std::string& textureFile, int width, int height, int cols, int rows)
        : spriteWidth(width), spriteHeight(height), numColumns(cols), numRows(rows), currentAnimation(0), currentFrame(0), frameTime(0.1f), isAttacking(false), position(0, 0) {
        if (!texture.loadFromFile(textureFile)) {
            std::cerr << "Failed to load texture: " << textureFile << std::endl;
            exit(1);
        }
        sprite.setTexture(texture);
        sprite.setTextureRect(sf::IntRect(0, 0, spriteWidth, spriteHeight));
        elapsedSeconds = 0.0f;
    }
    void addAnimation(const Animation& animation) { animations.push_back(animation); }
    void setPosition(float x, float y) { position.x = x; position.y = y; sprite.setPosition(x, y); }
    sf::Vector2f getPosition() const { return position; }
    void setScale(float x, float y) { sprite.setScale(x, y); }
    void setAnimation(const std::string& name) {
        for (size_t i = 0; i < animations.size(); ++i) {
            if (animations[i].getName() == name) {
                currentAnimation = i;
                currentFrame = 0;
                return;
            }
        }
        std::cerr << "Animation not found: " << name << std::endl;
    }
    void update(float deltaTime) {
        elapsedSeconds += deltaTime;
        if (elapsedSeconds >= frameTime) {
            elapsedSeconds -= frameTime;
            currentFrame++;
            if (currentFrame >= animations[currentAnimation].getNumFrames()) {
                currentFrame = 0;
                if (isAttacking) {
                    isAttacking = false;
                    setAnimation("walkRight");
                }
            }
            int frameIndex = animations[currentAnimation].getFrame(currentFrame);
            int frameX = (frameIndex % numColumns) * spriteWidth;
            int frameY = (frameIndex / numColumns) * spriteHeight;
            sprite.setTextureRect(sf::IntRect(frameX, frameY, spriteWidth, spriteHeight));
        }
    }
    void draw(sf::RenderWindow& window) { window.draw(sprite); }
    void attack() {
        if (!isAttacking) {
            isAttacking = true;
            setAnimation("attack");
            currentFrame = 0;
            elapsedSeconds = 0.0f;
        }
    }
    bool isAttackingNow() const { return isAttacking; }
};

class Enemy : public AnimatedSprite {
private:
    float moveSpeed;
    sf::Vector2f playerPosition;
    float attackRange;
    bool canAttack;
    int direction;
    sf::Vector2f patrolStart;
    sf::Vector2f patrolEnd;
    bool isPatrolling;
    std::string currentWalkAnimation;
public:
    Enemy(const std::string& textureFile, int width, int height, int cols, int rows)
        : AnimatedSprite(textureFile, width, height, cols, rows), moveSpeed(50.0f), attackRange(70.0f), canAttack(true), direction(1), isPatrolling(true), currentWalkAnimation("walkRight") {
        addAnimation(Animation("attack", {0, 1, 2, 3, 4, 5, 6, 7}));
        addAnimation(Animation("walkRight", {8, 9, 10, 11}));
        addAnimation(Animation("walkLeft", {12, 13, 14, 15}));
        setAnimation("walkRight");
        patrolStart = getPosition();
        patrolEnd = getPosition();
    }
    void setPlayerPosition(const sf::Vector2f& playerPos) { playerPosition = playerPos; }
    void setPatrolArea(float startX, float startY, float endX, float endY) {
        patrolStart.x = startX;
        patrolStart.y = startY;
        patrolEnd.x = endX;
        patrolEnd.y = endY;
        setPosition(patrolStart.x, patrolStart.y);
        isPatrolling = true;
    }
    void update(float deltaTime) {
        AnimatedSprite::update(deltaTime);
        sf::Vector2f enemyPosition = getPosition();
        float dx = playerPosition.x - enemyPosition.x;
        float dy = playerPosition.y - enemyPosition.y;
        float distanceToPlayer = sqrt(dx * dx + dy * dy);
        if (distanceToPlayer > attackRange) {
            if (isPatrolling) {
                patrol(deltaTime);
            } else {
                if (playerPosition.x > enemyPosition.x) {
                    position.x += moveSpeed * deltaTime;
                    if (currentWalkAnimation != "walkRight") {
                        setAnimation("walkRight");
                        currentWalkAnimation = "walkRight";
                    }
                } else if (playerPosition.x < enemyPosition.x) {
                    position.x -= moveSpeed * deltaTime;
                    if (currentWalkAnimation != "walkLeft") {
                        setAnimation("walkLeft");
                        currentWalkAnimation = "walkLeft";
                    }
                }
                if (playerPosition.y > enemyPosition.y) {
                    position.y += moveSpeed * deltaTime;
                } else if (playerPosition.y < enemyPosition.y) {
                    position.y -= moveSpeed * deltaTime;
                }
                setPosition(position.x, position.y);
            }
        } else if (distanceToPlayer <= attackRange && canAttack) {
            attack();
            isPatrolling = false;
        } else {
            if (!isAttackingNow())
                setAnimation("walkRight");
        }
    }
    void draw(sf::RenderWindow& window) { window.draw(sprite); }
    bool canAttackPlayer() const { return canAttack; }
    void setCanAttack(bool value) { canAttack = value; }
    float calculateDistance(float dx, float dy) { return sqrt(dx * dx + dy * dy); }
    void patrol(float deltaTime) {
        if (direction == 1) {
            position.x += moveSpeed * deltaTime;
            if (currentWalkAnimation != "walkRight") {
                setAnimation("walkRight");
                currentWalkAnimation = "walkRight";
            }
            if (position.x >= patrolEnd.x) {
                direction = -1;
            }
        } else if (direction == -1) {
            position.x -= moveSpeed * deltaTime;
            if (currentWalkAnimation != "walkLeft") {
                setAnimation("walkLeft");
                currentWalkAnimation = "walkLeft";
            }
            if (position.x <= patrolStart.x) {
                direction = 1;
            }
        }
        setPosition(position.x, position.y);
    }
};

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Minion AKA Enemy");
    Enemy enemy1("attack - sword shield.png", 66, 67, 4, 4);
    enemy1.setPosition(400, 300);
    enemy1.setPatrolArea(200, 100, 300, 200);
    sf::Clock clock;
    sf::Vector2f playerPosition(100, 100);
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        float deltaTime = clock.restart().asSeconds();
        enemy1.setPlayerPosition(playerPosition);
        enemy1.update(deltaTime);
        window.clear(sf::Color::Black);
        enemy1.draw(window);
        window.display();
    }
    return 0;
}
