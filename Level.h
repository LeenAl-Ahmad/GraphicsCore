#ifndef LEVEL_H
#define LEVEL_H

#include "StandardIncludes.h"
#include "Renderer.h"
#include "SpriteSheet.h"
#include "Timing.h"
#include "TTFont.h"

class Level {
public:
    Level();
    ~Level();

    void Initialize(Renderer* renderer);
    void Update(float deltaTime);
    void Render(Renderer* renderer, Timing* timing);
    bool IsComplete();

private:
    TTFont* font;          // Pointer to the font used for rendering text
    SpriteSheet* sheet;    // Pointer to the sprite sheet for animations
};

#endif // LEVEL_H