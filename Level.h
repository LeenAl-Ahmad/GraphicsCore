#ifndef LEVEL_H
#define LEVEL_H

#include "Renderer.h"
#include "Unito.h"
#include "Resource.h"
#include "Timing.h"
#include "PhysicsController.h"
#include "InputController.h"
#include "GameController.h"
#include "Keyboard.h"
#include "Mouse.h"

class Level : public Resource
{
public:
    Level();
    ~Level();

    void AssignNonDefaultValues() override;
    void Render(Renderer* renderer, Timing* timing);
    void HandleInput(SDL_Event event, float deltaTime);
    void Update(float deltaTime);
    void TagNearbyNPCs();
    void Serialize(std::ostream& _stream);
    void Deserialize(std::istream& _stream);

float npcSpeed = 60.0f;
private:
    Unit* player;
    PhysicsController* physics;
    InputController* input;
    bool m_quit;
    const float moveForce = 500.0f; 
    const float maxSpeed = 100.0f;
    std::vector<Unit*> npcWarriors;
    std::vector<bool> npcTagged;
    std::vector<float> npcDeathTimers;
    const float DEATH_ANIMATION_DURATION = 1.0f;
      // Add this with your other member variables
    const float MAX_NPC_SPEED = 60.0f;
    const float MIN_NPC_SPEED = 0.0f;
    const float SPEED_INCREMENT = 10.0f;
};
#endif