#ifndef LEVEL_H
#define LEVEL_H

#include "StandardIncludes.h"
#include "Renderer.h"
#include "SpriteSheet.h"
#include "Timing.h"
#include "TTFont.h"

class Level : public  SpriteSheet{
public:
    Level(int levelNumber = 1); // Constructor with default level number
    ~Level();

    void Initialize(Renderer* renderer);
    void Update(float deltaTime);
    void Render(Renderer* renderer, Timing* timing);
    bool IsComplete();
    
    void Serialize(std::ostream& _stream) override;
    void Deserialize(std::istream& _stream) override;

private:
    TTFont* font;
    SpriteSheet* sheet;
    SpriteSheet* rockSheet; 
    Uint32 startTime;
    bool isCompleted;
    int levelNumber;

    struct Warrior {
        float x, y;
        float speed;
        float animationSpeed;
        int currentFrame;  // The current frame in the animation loop
        float deathAnimationTime;
        int state; //0 = alive, 1 = dying, 2 = dead
        int deathAnimationFrame;
        Warrior() : x(0), y(0), speed(0), animationSpeed(1.0f), currentFrame(0){}
    };

    struct Rock {
        float x, y; // Position
        float speed; // Falling speed
        float animationSpeed; // Animation speed
        int currentFrame; // Current animation frame
    };

    // In Level.h, add a vector for rocks
    std::vector<Rock> rocks;
    std::vector<Warrior> warriors; // List of warriors
    bool autoSaved; // Track if the level has been auto-saved
    Uint32 gameTime; // Track game time in seconds
    int run = 8;
};

#endif // LEVEL_H