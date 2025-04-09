#include "Level.h"

// Level Implementation
Level::Level(PhysicsController* physics) : m_physics(physics) {
    m_spawnAreaMin = glm::vec2(16, 16);
    m_spawnAreaMax = glm::vec2(1920 - 32, 1080 - 32);
    m_destinationAreaMin = glm::vec2(0, 0);
    m_destinationAreaMax = glm::vec2(1920, 1080);

    m_circleSprite = SpriteSheet::Pool->GetResource();
    m_circleSprite->Load("C:/Users/leana/source/repos/GraphicsCore/Assets/Textures/Circle.tga");
    m_circleSprite->SetSize(1, 1, 32, 32);
    m_circleSprite->AddAnimation(EN_AN_IDLE, 0, 1, 0.0f);
    m_circleSprite->SetBlendMode(SDL_BLENDMODE_BLEND);
}

Level::~Level() {
    if (m_circleSprite) {
        SpriteSheet::Pool->ReleaseResource(m_circleSprite);
    }
}

void Level::AddPhysicsObject() {
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

void Level::Render(Renderer* renderer, Timing* timing) {
    Rect r = m_circleSprite->Update(EN_AN_IDLE, timing->GetDeltaTime());
    for (RigidBody* p : m_physics->GetBodies()) {
        auto pos = p->GetPosition();
        renderer->RenderTexture(m_circleSprite, r,
            Rect(pos.x - 16, pos.y - 16, pos.x + 16, pos.y + 16),
            p->GetMass() + 127);
    }
}