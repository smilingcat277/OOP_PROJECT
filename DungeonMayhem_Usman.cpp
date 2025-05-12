#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <stdexcept>

class Animation {
public:
    std::string name;
    std::vector<std::pair<int, int>> frames; // (row, col)

    Animation(const std::string& animationName, const std::vector<std::pair<int, int>>& frameCoords)
        : name(animationName), frames(frameCoords) {}

    std::pair<int, int> getFrame(int index) const {
        if (index >= 0 && index < frames.size()) {
            return frames[index];
        }
        // This warning is fine to keep, indicating a logic issue but not crashing
        std::cerr << "Warning: Animation frame index out of bounds for animation: " << name << " at frame " << index << std::endl;
        if (!frames.empty()) return frames[0];
        return {0, 0};
    }

    int getNumFrames() const {
        return frames.size();
    }

    const std::string& getName() const {
        return name;
    }
};


class AnimatedSprite {
protected:
    sf::Texture attackTexture;
    sf::Texture walkTexture;
    sf::Texture deathTexture;
    sf::Texture* currentTexture;

    int walkAttackSpriteWidth;
    int walkAttackSpriteHeight;
    int walkAttackNumColumns;

    int deathSpriteWidth;
    int deathSpriteHeight;
    int deathNumColumns;

    int currentSpriteWidth;
    int currentSpriteHeight;
    int currentNumColumns;


    std::vector<Animation> animations;
    int currentAnimationIndex;
    int currentFrame;
    float frameTime;
    float elapsedSeconds;
    std::string currentAnimationName;
    sf::Sprite sprite;
    bool animationLoops;
    bool isPlaying = true;

public:
    sf::Vector2f position;

    int getCurrentSpriteWidth() const { return currentSpriteWidth; }
    int getCurrentSpriteHeight() const { return currentSpriteHeight; }
    int getCurrentNumColumns() const { return currentNumColumns; }


    AnimatedSprite(const std::string& attackTextureFile, const std::string& walkTextureFile, const std::string& deathTextureFile,
                   int walkAtkWidth, int walkAtkHeight, int walkAtkCols,
                   int deathWidth, int deathHeight, int deathCols)
        : walkAttackSpriteWidth(walkAtkWidth), walkAttackSpriteHeight(walkAtkHeight), walkAttackNumColumns(walkAtkCols),
          deathSpriteWidth(deathWidth), deathSpriteHeight(deathHeight), deathNumColumns(deathCols),
          currentAnimationIndex(0), currentFrame(0), frameTime(0.1f), elapsedSeconds(0),
          position(0, 0), currentAnimationName(""), animationLoops(true)
    {
        // Throw exceptions if texture loading fails
        if (!attackTexture.loadFromFile(attackTextureFile)) {
            throw std::runtime_error("Failed to load attack texture: " + attackTextureFile);
        }
        if (!walkTexture.loadFromFile(walkTextureFile)) {
            throw std::runtime_error("Failed to load walk texture: " + walkTextureFile);
        }
        if (!deathTexture.loadFromFile(deathTextureFile)) {
            throw std::runtime_error("Failed to load death texture: " + deathTextureFile);
        }

        currentTexture = &walkTexture;
        currentSpriteWidth = walkAttackSpriteWidth;
        currentSpriteHeight = walkAttackSpriteHeight;
        currentNumColumns = walkAttackNumColumns;


        sprite.setTexture(*currentTexture);
        sprite.setTextureRect(sf::IntRect(0, 0, currentSpriteWidth, currentSpriteHeight));
    }

    void addAnimation(const Animation& anim) {
        animations.push_back(anim);
        if (animations.empty()) { // Should not happen if push_back succeeds, but check anyway
             std::cerr << "Warning: Failed to add animation " << anim.getName() << std::endl;
             return;
        }
        // If this is the *first* animation added, set it as current default
        if (animations.size() == 1 && currentAnimationName.empty()) {
             setAnimation(anim.getName(), true);
        }
    }

    void setAnimation(const std::string& name, bool loops = true) {
        // Prevent resetting if same animation is already set and intended to play
        if (currentAnimationName == name && isPlaying) {
            return;
        }

        size_t foundIndex = -1; // Use size_t for index comparison
        for (size_t i = 0; i < animations.size(); ++i) {
            if (animations[i].getName() == name) {
                foundIndex = i;
                break;
            }
        }

        if (foundIndex == (size_t)-1) { // Check if animation was found
             std::cerr << "Warning: Attempted to set unknown animation: '" << name << "'" << std::endl;
             // Optional: set to a default idle animation if available
             return; // Do not change animation state if name is invalid
        }

        currentAnimationIndex = foundIndex;
        currentFrame = 0;
        elapsedSeconds = 0;
        currentAnimationName = name;
        animationLoops = loops;
        isPlaying = true; // Start playing the new animation

        // Set the correct texture and dimensions based on animation name
        if (name.find("death") != std::string::npos) {
            currentTexture = &deathTexture;
            currentSpriteWidth = deathSpriteWidth;
            currentSpriteHeight = deathSpriteHeight;
            currentNumColumns = deathNumColumns;
        } else if (name.find("attack") != std::string::npos) {
             currentTexture = &attackTexture;
             currentSpriteWidth = walkAttackSpriteWidth;
             currentSpriteHeight = walkAttackSpriteHeight;
             currentNumColumns = walkAttackNumColumns;
        } else { // Default to walk textures if not death or attack
            currentTexture = &walkTexture;
            currentSpriteWidth = walkAttackSpriteWidth;
            currentSpriteHeight = walkAttackSpriteHeight;
            currentNumColumns = walkAttackNumColumns;
        }

        sprite.setTexture(*currentTexture, true);
        // Set texture rect for the initial frame immediately
        if (!animations[currentAnimationIndex].frames.empty()) {
             auto [row, col] = animations[currentAnimationIndex].getFrame(currentFrame);
             int x = col * currentSpriteWidth;
             int y = row * currentSpriteHeight;
             sprite.setTextureRect(sf::IntRect(x, y, currentSpriteWidth, currentSpriteHeight));
        } else {
            sprite.setTextureRect(sf::IntRect(0, 0, currentSpriteWidth, currentSpriteHeight)); // Fallback
        }
    }

    void setPosition(float x, float y) {
        position = { x, y };
        sprite.setPosition(position);
    }

    sf::Vector2f getPosition() const {
        return position;
    }

    bool update(float deltaTime) {
        bool animationFinished = false;

        // If not playing or no animations are defined, just return
        if (!isPlaying || animations.empty() || currentAnimationIndex >= animations.size() ) {
             isPlaying = false; // Ensure isPlaying is false if no valid animation is set
             return false;
        }

        // Ensure current frame is within bounds if state was somehow corrupted
        if (currentFrame >= animations[currentAnimationIndex].getNumFrames()) {
            if (animationLoops && animations[currentAnimationIndex].getNumFrames() > 0) {
                 currentFrame = currentFrame % animations[currentAnimationIndex].getNumFrames(); // Wrap around
            } else {
                 currentFrame = animations[currentAnimationIndex].getNumFrames() - 1; // Clamp to last frame
                 isPlaying = false; // Stop if clamped and not looping
            }
             // Recalculate rect for clamped/wrapped frame
             if (currentFrame >= 0 && currentFrame < animations[currentAnimationIndex].getNumFrames()) {
                 auto [row, col] = animations[currentAnimationIndex].getFrame(currentFrame);
                 int x = col * currentSpriteWidth;
                 int y = row * currentSpriteHeight;
                 sprite.setTextureRect(sf::IntRect(x, y, currentSpriteWidth, currentSpriteHeight));
             }
             // If still not playing after adjustment, return
             if (!isPlaying) return true; // It just finished or was already finished
        }


        elapsedSeconds += deltaTime;

        if (elapsedSeconds >= frameTime) {
            elapsedSeconds -= frameTime;
            int framesToAdvance = 1; // Can calculate based on elapsedSeconds/frameTime if lag occurs
            // For simplicity, we just advance 1 frame per frameTime interval
            // int framesToAdvance = static_cast<int>(elapsedSeconds / frameTime);
            // elapsedSeconds = std::fmod(elapsedSeconds, frameTime); // Keep remainder

            currentFrame += framesToAdvance;


            if (currentFrame >= animations[currentAnimationIndex].getNumFrames()) {
                if (animationLoops) {
                    currentFrame = currentFrame % animations[currentAnimationIndex].getNumFrames(); // Loop back
                } else {
                    currentFrame = animations[currentAnimationIndex].getNumFrames() - 1; // Stay on last frame
                    isPlaying = false; // Stop animation on last frame if not looping
                    animationFinished = true; // Signal completion
                }
            }

             // Update texture rect for the new frame
             if (currentFrame >= 0 && currentFrame < animations[currentAnimationIndex].getNumFrames()) {
                 auto [row, col] = animations[currentAnimationIndex].getFrame(currentFrame);
                 int x = col * currentSpriteWidth;
                 int y = row * currentSpriteHeight;
                 sprite.setTextureRect(sf::IntRect(x, y, currentSpriteWidth, currentSpriteHeight));
             }
        }

        return animationFinished;
    }

    void draw(sf::RenderWindow& window) {
        window.draw(sprite);
    }

    std::string getCurrentAnimationName() const {
        return currentAnimationName;
    }

    void stopAnimation() {
        isPlaying = false;
    }

    void playAnimation() {
        // Only set to playing if there is a valid animation set
        if (!animations.empty() && currentAnimationIndex < animations.size()) {
             isPlaying = true;
        }
    }

    bool getIsPlaying() const {
        return isPlaying;
    }

};

enum class EnemyState {
    Patrolling,
    Attacking,
    Dying,
    Dead
};

class Enemy : public AnimatedSprite {
private:
    float moveSpeed;
    sf::Vector2f playerPos; // Player position used for targeting
    float attackRange;
    sf::Clock attackCooldownClock;
    bool patrolVertical;
    sf::Vector2f patrolStart, patrolEnd;
    int direction;

    int health; // Enemy's health
    EnemyState currentState;

public:
     Enemy(const std::string& atkTex, const std::string& walkTex, const std::string& deathTex,
          int walkAtkWidth, int walkAtkHeight, int walkAtkCols, int deathWidth, int deathHeight, int deathCols)
        : AnimatedSprite(atkTex, walkTex, deathTex, walkAtkWidth, walkAtkHeight, walkAtkCols, deathWidth, deathHeight, deathCols),
        moveSpeed(50.f), attackRange(70.f), patrolVertical(false), direction(1),
        health(3), currentState(EnemyState::Patrolling)
    {

        // --- Define Animations ---
        
        addAnimation(Animation("walkDown",  { {0,0},{0,1},{0,2},{0,3} }));
        addAnimation(Animation("walkUp",    { {1,0},{1,1},{1,2},{1,3} }));
        addAnimation(Animation("walkRight", { {2,0},{2,1},{2,2},{2,3} }));
        addAnimation(Animation("walkLeft",  { {3,0},{3,1},{3,2},{3,3} }));

        addAnimation(Animation("attackDown", { {0,0},{0,1},{0,2},{0,3} }));
        addAnimation(Animation("attackUp",   { {1,0},{1,1},{1,2},{1,3} }));
        addAnimation(Animation("attackRight",{ {2,0},{2,1},{2,2},{2,3} }));
        addAnimation(Animation("attackLeft", { {3,0},{3,1},{3,2},{3,3} }));
        
        std::vector<std::pair<int, int>> deathFrames;
        for(int i = 0; i < deathCols; ++i) {
            deathFrames.push_back({0, i});
        }
        addAnimation(Animation("death", deathFrames));

        // setAnimation is called automatically for the first added animation (walkDown)
    }

    // This method should be called by the player's attack logic
    void takeDamage(int amount) {
        if (currentState == EnemyState::Dying || currentState == EnemyState::Dead) {
            return;
        }

        health -= amount;
        std::cout << "Enemy took damage, health: " << health << std::endl;

        if (health <= 0) {
            currentState = EnemyState::Dying;
            setAnimation("death", false);
            std::cout << "Enemy state changed to Dying" << std::endl;
        }
    }

    // This method is for the enemy's AI to know where the player is
    void setPlayerPosition(const sf::Vector2f& pos) {
        playerPos = pos;
    }

    void setPatrol(bool vertical, sf::Vector2f start, sf::Vector2f end) {
        patrolVertical = vertical;
        patrolStart = start;
        patrolEnd = end;
        setPosition(start.x, start.y);
        // Ensure correct animation is set when patrol starts
        if (patrolVertical) {
             setAnimation(direction == 1 ? "walkDown" : "walkUp", true);
        } else {
             setAnimation(direction == 1 ? "walkRight" : "walkLeft", true);
        }
        currentState = EnemyState::Patrolling; // Explicitly set state when patrol is configured
    }

    EnemyState getCurrentState() const {
        return currentState;
    }


    void update(float dt) {
        // Only update animation if the enemy is not yet Dead
        // The update method itself handles stopping animation in Dying state
        bool animJustFinished = false;
        if (currentState != EnemyState::Dead) {
           animJustFinished = AnimatedSprite::update(dt);
        }


        switch (currentState) {
            case EnemyState::Patrolling: {
                 // Re-set walk animation if it got interrupted or wasn't playing
                 if (getCurrentAnimationName().find("walk") == std::string::npos || !getIsPlaying()) {
                       if (patrolVertical) {
                             setAnimation(direction == 1 ? "walkDown" : "walkUp", true);
                       } else {
                             setAnimation(direction == 1 ? "walkRight" : "walkLeft", true);
                       }
                 }

                sf::Vector2f delta = playerPos - position;
                float dist = std::sqrt(delta.x * delta.x + delta.y * delta.y);

                if (dist <= attackRange && attackCooldownClock.getElapsedTime().asSeconds() >= 1.f) {
                    currentState = EnemyState::Attacking;
                    std::string dir;
                    if (std::abs(delta.x) > std::abs(delta.y)) {
                        dir = delta.x > 0 ? "Right" : "Left";
                    } else {
                        dir = delta.y > 0 ? "Down" : "Up";
                    }
                    setAnimation("attack" + dir, false);
                    attackCooldownClock.restart();
                } else {
                    patrol(dt);
                }
                break;
            }

            case EnemyState::Attacking: {
                if (animJustFinished) {
                     stopAnimation(); // Ensure animation is stopped after non-looping finishes

                     sf::Vector2f delta = playerPos - position;
                     float dist = std::sqrt(delta.x * delta.x + delta.y * delta.y);

                     if (dist <= attackRange && attackCooldownClock.getElapsedTime().asSeconds() >= 1.f) {
                         std::string dir;
                         if (std::abs(delta.x) > std::abs(delta.y)) {
                             dir = delta.x > 0 ? "Right" : "Left";
                         } else {
                             dir = delta.y > 0 ? "Down" : "Up";
                         }
                         setAnimation("attack" + dir, false);
                         attackCooldownClock.restart();
                     } else {
                         currentState = EnemyState::Patrolling;
                         if (patrolVertical) {
                              setAnimation(direction == 1 ? "walkDown" : "walkUp", true);
                         } else {
                              setAnimation(direction == 1 ? "walkRight" : "walkLeft", true);
                         }
                     }
                }
                break;
            }

            case EnemyState::Dying: {
                if (animJustFinished) {
                    currentState = EnemyState::Dead;
                    stopAnimation(); // Ensure animation is stopped
                    std::cout << "Enemy state changed to Dead" << std::endl;
                }
                break;
            }

            case EnemyState::Dead: {
                // In the Dead state, the enemy does nothing and does not animate
                // stopAnimation() is called when entering this state
                break; // No logic needed while dead
            }
        }
    }

    void patrol(float dt) {
        if (patrolVertical) {
            position.y += direction * moveSpeed * dt;
            if (direction == 1 && position.y >= patrolEnd.y) {
                position.y = patrolEnd.y;
                direction = -1;
                setAnimation("walkUp", true);
            } else if (direction == -1 && position.y <= patrolStart.y) {
                position.y = patrolStart.y;
                direction = 1;
                setAnimation("walkDown", true);
            }
        } else { // Horizontal patrol
            position.x += direction * moveSpeed * dt;
            if (direction == 1 && position.x >= patrolEnd.x) {
                position.x = patrolEnd.x;
                direction = -1;
                setAnimation("walkLeft", true);
            } else if (direction == -1 && position.x <= patrolStart.x) {
                position.x = patrolStart.x;
                direction = 1;
                setAnimation("walkRight", true);
            }
        }
        setPosition(position.x, position.y);
    }

    bool isDead() const {
        return currentState == EnemyState::Dead;
    }

    void draw(sf::RenderWindow& window) {
        if (currentState != EnemyState::Dead) {
            AnimatedSprite::draw(window);
        }
    }
};

int main() {
    // Wrap the game setup in a try block
    try {
        sf::RenderWindow window(sf::VideoMode(800, 600), "Enemy Patrol, Attack, and Death");
        window.setFramerateLimit(60);

        const int WALK_ATTACK_FRAME_WIDTH = 64;
        const int WALK_ATTACK_FRAME_HEIGHT = 66;
        const int WALK_ATTACK_COLUMNS = 4;

        const int DEATH_FRAME_WIDTH = 64;
        const int DEATH_FRAME_HEIGHT = 66;
        const int DEATH_COLUMNS = 8;

        Enemy enemy("attack - sword shield.png",
                    "walk - sword shield.png",
                    "imp death.png",
                    WALK_ATTACK_FRAME_WIDTH, WALK_ATTACK_FRAME_HEIGHT, WALK_ATTACK_COLUMNS,
                    DEATH_FRAME_WIDTH, DEATH_FRAME_HEIGHT, DEATH_COLUMNS);

        enemy.setPatrol(true, { 300, 100 }, { 300, 500 });
        // enemy.setPatrol(false, { 100, 300 }, { 700, 300 });
        
        sf::Clock clock;
        // This playerPos is just for the enemy's AI to track;
        sf::Vector2f playerPos(400, 300); // Fixed initial position


        // Main game loop only runs if setup was successful
        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    window.close();
            }

            float dt = clock.restart().asSeconds();

            // (This playerPos variable will need to be updated by your actual player object)
            enemy.setPlayerPosition(playerPos);

            // Update the enemy's state, animation, and movement
            enemy.update(dt);

            window.clear(sf::Color::Black);
            enemy.draw(window);

            window.display();
        }

        return 0;

    // Catch specific runtime errors (like texture loading failures)
    } catch (const std::runtime_error& e) {
        std::cerr << "Runtime error during game setup: " << e.what() << std::endl;
        return 1; // Return non-zero to indicate an error occurred

    } catch (const std::exception& e) {
        std::cerr << "An unexpected standard exception occurred: " << e.what() << std::endl;
        return 1;
        
    } catch (...) {
        std::cerr << "An unknown error occurred during game setup." << std::endl;
        return 1;
    }
}
