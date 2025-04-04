#include "Units.h"

Unit::Unit(const std::string& texturePath,
    int rows, int cols,
    int frameWidth, int frameHeight)
    : m_tint({ 255, 255, 255, 255 }),
    m_currentAnim(EN_AN_IDLE)
{
    // Create and configure sprite sheet in one go
    m_spriteSheet = SpriteSheet::Pool->GetResource();
    m_spriteSheet->Load(texturePath);
    m_spriteSheet->SetSize(rows, cols, frameWidth, frameHeight);

    // Default animations (can be overridden)
    m_spriteSheet->AddAnimation(EN_AN_IDLE, 0, 1, 0.1f);
}

void Unit::Update(float deltaTime) {
    m_body.Update(deltaTime, glm::vec2(0, 0));
}

void Unit::Render() {
    Rect frame = m_spriteSheet->Update(m_currentAnim, 0.016f);
    glm::vec2 pos = m_body.GetPosition();

    Renderer::Instance().RenderTexture(
        m_spriteSheet,
        frame,
        Rect(pos.x - 32, pos.y - 32, 64, 64),
        m_tint.a
    );
}

void Unit::SetPosition(float x, float y) {
    m_body.SetPosition(glm::vec2(x, y));
}

void Unit::SetVelocity(float x, float y) {
    m_body.SetVelocity(glm::vec2(x, y));
}

void Unit::SetTint(Uint8 r, Uint8 g, Uint8 b) {
    m_tint = { r, g, b, 255 };
}

void Unit::AddAnimation(AnimationNames name, int startFrame, int frameCount, float speed) {
    m_spriteSheet->AddAnimation(name, startFrame, frameCount, speed);
}

void Unit::PlayAnimation(AnimationNames name) {
    m_currentAnim = name;
}