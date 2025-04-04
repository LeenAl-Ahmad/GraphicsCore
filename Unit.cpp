#include "Unit.h"
#include <SDL.h>
#include <cassert>

ObjectPool<Unit>* Unit::Pool = nullptr;

Unit::Unit() :
    m_spriteSheet(nullptr),
    m_swooshSound(nullptr)
{
    assert(SpriteSheet::Pool != nullptr && "SpriteSheet pool must be initialized first");
    m_spriteSheet = SpriteSheet::Pool->GetResource();

    // Set up sprite sheet (assuming Load() was called elsewhere)
    m_spriteSheet->SetSize(17, 6, 69, 44);
    m_spriteSheet->AddAnimation(EN_AN_IDLE, 0, 6, 0.1f);
    m_spriteSheet->AddAnimation(EN_AN_RUN, 6, 8, 0.08f);
    m_spriteSheet->AddAnimation(EN_AN_DEATH, 26, 11, 0.08f);

    // Load sound
    m_swooshSound = AudioController::Instance().LoadEffect("./Assets/Audio/Effects/Whoosh.wav");
    assert(m_swooshSound != nullptr && "Failed to load swoosh sound");
}

Unit::~Unit() {
    // Sound cleanup handled by AudioController
}

void Unit::Update(float deltaTime) {
    if (m_isTagged) {
        if (m_spriteSheet->GetCurrentClip(EN_AN_DEATH) >= 36.0f - FLT_EPSILON) {
            Unit::Pool->ReleaseResource(this);
        }
        return;
    }
    RigidBody::Update(deltaTime, glm::vec2(0, 0));
}

void Unit::Render(SDL_Renderer* renderer) {
    if (!renderer || !m_spriteSheet) return;

    // Determine animation state
    AnimationNames anim = m_isTagged ? EN_AN_DEATH :
        (glm::length(GetVelocity()) > 0.1f ? EN_AN_RUN : EN_AN_IDLE);

    // Get current frame
    Rect frame = m_spriteSheet->Update(anim, 0.016f);

    // Calculate scaled dimensions
    const float scale = 1.25f;
    const float width = 32.0f * scale;
    const float height = 32.0f * scale;
    glm::vec2 pos = GetPosition();

    // Render through existing Renderer instance
    m_renderer->RenderTexture(
        m_spriteSheet,
        frame,
        Rect(pos.x - width / 2, pos.y - height / 2, width, height)
    );
}

void Unit::Tag() {
    if (m_isTagged) return;
    m_isTagged = true;
    AudioController::Instance().Play(m_swooshSound);
}