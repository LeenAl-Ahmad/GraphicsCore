#include "Level.h"

// Level Implementation
Level::Level(PhysicsController* physics) : m_physics(physics) {
    m_spawnAreaMin = glm::vec2(16, 16);
    m_spawnAreaMax = glm::vec2(1920 - 32, 1080 - 32);
    m_destinationAreaMin = glm::vec2(0, 0);
    m_destinationAreaMax = glm::vec2(1920, 1080);
    
}

Level::~Level() {

}

void Level::Render(Renderer* renderer, Timing* timing) {
    
}

void Level::AddUnit() {
    // Create physics object first
    AddPhysicsObject();

    // Get the last added physics body
    RigidBody* body = m_physics->GetBodies().back();
    glm::vec2 pos = body->GetPosition();

    // Get sprite from pool
    SpriteSheet* sprite = SpriteSheet::Pool->GetResource();
    sprite->Load("C:/Users/leana/source/repos/GraphicsCore/Assets/Textures/Circle.tga");
    sprite->SetSize(1, 1, 32, 32);
    sprite->AddAnimation(EN_AN_IDLE, 0, 1, 0.0f);
    sprite->SetBlendMode(SDL_BLENDMODE_BLEND);

    // Create unit with this physics body
    Unit* newUnit = new Unit(sprite, body); // Modified Unit constructor
    m_units.push_back(newUnit);
}

void Level::AddPhysicsObject() {
    // Keep your existing physics creation logic
    glm::vec2 pos = glm::vec2{
        m_spawnAreaMin.x + rand() % (int)(m_spawnAreaMax.x - m_spawnAreaMin.x),
        m_spawnAreaMin.y + rand() % (int)(m_spawnAreaMax.y - m_spawnAreaMin.y)
    };

    glm::vec2 dest = glm::vec2{
        m_destinationAreaMin.x + rand() % (int)(m_destinationAreaMax.x - m_destinationAreaMin.x),
        m_destinationAreaMin.y + rand() % (int)(m_destinationAreaMax.y - m_destinationAreaMin.y)
    };

    glm::vec2 dir = dest - pos;
    dir = glm::normalize(dir) * 200.0f;

    m_physics->AddRigidBody(pos, dir, rand() % 128);
}
