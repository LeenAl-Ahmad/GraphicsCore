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
    void Save(const std::string& filename);

private:
    TTFont* font;          
    SpriteSheet* sheet;
    Uint32 startTime; 
    bool isCompleted;     
};

#endif // LEVEL_H