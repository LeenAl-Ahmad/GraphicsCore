#pragma once
#include "Renderer.h"
#include "TTFont.h"
#include "InputController.h"
#include "PhysicsController.h"
#include "Timing.h"
#include "SpriteSheet.h"
#include "SpriteAnim.h"
#include "RigidBody.h"

class Level {
private:
    PhysicsController* m_physics;
    SpriteSheet* m_circleSprite;

    glm::vec2 m_spawnAreaMin;
    glm::vec2 m_spawnAreaMax;
    glm::vec2 m_destinationAreaMin;
    glm::vec2 m_destinationAreaMax;

public:
    Level(PhysicsController* physics);
    ~Level();

    void AddPhysicsObject();
    void Render(Renderer* renderer, Timing* timing);
};