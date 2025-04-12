#pragma once
#include "Renderer.h"
#include "TTFont.h"
#include "InputController.h"
#include "PhysicsController.h"
#include "Timing.h"
#include "SpriteSheet.h"
#include "SpriteAnim.h"
#include "RigidBody.h"
#include "Unit.h"
#include "Player.h"

class Level {

public:
    Level(PhysicsController* physics);
    ~Level();

    void AddPhysicsObject();
    void Render(Renderer* renderer, Timing* timing);
    void AddUnit();
    size_t GetUnitCount() const { return m_units.size(); }

private:
    PhysicsController* m_physics;
    SpriteSheet* m_circleSprite;

    glm::vec2 m_spawnAreaMin;
    glm::vec2 m_spawnAreaMax;
    glm::vec2 m_destinationAreaMin;
    glm::vec2 m_destinationAreaMax;

    std::vector<Unit*> m_units;
    Texture* m_texture = nullptr;

    Player* m_player = nullptr;
};