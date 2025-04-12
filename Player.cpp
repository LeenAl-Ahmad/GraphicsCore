#include "Player.h"
#include "SpriteSheet.h"
#include "Renderer.h"
#include "PhysicsController.h"

Player::Player()
{
    m_state = PlayerState::GetNormState();

    m_rigidBody = PhysicsController::Instance().AddRigidBody(glm::vec2{ 100, 200 }, glm::vec2(0, 0), 40);
    m_animations = SpriteSheet::Pool->GetResource();
    m_animations->Load("./Assets/Textures/RockPaperScissors.tga");
    m_animations->SetSize(4, 4, 1000, 1900);
    m_animations->AddAnimation(EN_AN_IDLE, 0, 0, 10.0f);
    m_animations->AddAnimation(EN_AN_ROLL, 0, 3, 4.0f);
    m_animations->AddAnimation(EN_AN_ROCK, 4, 7, 8.0f);
    m_animations->AddAnimation(EN_AN_PAPER, 8, 11, 8.0f);
    m_animations->AddAnimation(EN_AN_SCISSOR, 12, 15, 8.0f);
    m_animations->SetBlendMode(SDL_BLENDMODE_BLEND);
}

Player::~Player()
{
}

void Player::HandleInput(SDL_Event _event, float _deltaTime)
{
    m_state->HandleInput(this, _event);
}

void Player::Update(float _deltaTime)
{
    m_state->Update(this, _deltaTime);
}

void Player::Render(Renderer* _renderer)
{
    glm::vec2 p = m_rigidBody->GetPosition();
    Rect destRect = Rect(p.x, p.y, p.x + 69 * 2, p.y + 44 * 2);
    _renderer->RenderTexture(m_animations, m_srcRect, destRect);
}
