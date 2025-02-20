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
        float x; // X position
        float y; // Y position
        float speed; // Running speed (pixels per second)
        float animationSpeed; // Animation speed
        int currentFrame; // Current frame of the "Run" animation
    };

    std::vector<Warrior> warriors; // List of warriors
    bool autoSaved; // Track if the level has been auto-saved
    Uint32 gameTime; // Track game time in seconds
};

#endif // LEVEL_H