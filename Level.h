#ifndef LEVEL_H
#define LEVEL_H

#include "Renderer.h"
#include "Unito.h"
#include "Resource.h"
#include "Timing.h"
#include "PhysicsController.h"
#include "GameController.h"

class Level : public Resource
{
public:
    Level();
    ~Level();

    void AssignNonDefaultValues() override;
    void Render(Renderer* renderer, Timing* timing);
    void HandleInput(SDL_Event event); 
    void Update(float deltaTime);

private:
    Unit* player;
    PhysicsController* physics;
};
#endif