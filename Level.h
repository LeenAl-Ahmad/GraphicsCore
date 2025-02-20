#ifndef LEVEL_H
#define LEVEL_H

#include "StandardIncludes.h"
#include "Renderer.h"
#include "SpriteSheet.h"
#include "Timing.h"
#include "TTFont.h"

class Level {
public:
    Level(int levelNumber = 1); // Constructor with default level number
    ~Level();

    void Initialize(Renderer* renderer);
    void Update(float deltaTime);
    void Render(Renderer* renderer, Timing* timing);
    bool IsComplete();
    void Save(const std::string& filename);
    void Load(const std::string& filename);

private:
    TTFont* font;
    SpriteSheet* sheet;
    Uint32 startTime;
    bool isCompleted;
    int levelNumber;

    struct Warrior {
        float x, y;
        float speed;
        float animationSpeed;
        int currentFrame;  // The current frame in the animation loop
        int animationClip; // This could store which animation clip to use (EN_AN_RUN, etc.)

        Warrior() : x(0), y(0), speed(0), animationSpeed(1.0f), currentFrame(0), animationClip(EN_AN_RUN) {}
    };


    std::vector<Warrior> warriors; // List of warriors
    bool autoSaved; // Track if the level has been auto-saved
    Uint32 gameTime; // Track game time in seconds
    int run = 8;
};

#endif // LEVEL_H