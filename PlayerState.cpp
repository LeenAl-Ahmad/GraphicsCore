#include "PlayerState.h"
#include "InputController.h"
#include "Keyboard.h"
#include "Player.h"
#include "SpriteSheet.h"

NormState PlayerState::m_norm;
RollState PlayerState::m_roll;
RockState PlayerState::m_rock;
PaperState PlayerState::m_paper;
ScissorState PlayerState::m_scissor;

void NormState::HandleInput(Player* _p, SDL_Event _e)
{
    if (InputController::Instance().KB()->KeyDown(_e, SDLK_SPACE)) {
        _p->SetState(PlayerState::GetRollState());
    }
}

void NormState::Update(Player* _p, float _dt)
{
    _p->GetAnimations()->Update(EN_AN_IDLE, _dt, _p->GetSrcRect());
}

void RollState::HandleInput(Player* _p, SDL_Event _e)
{
    //no input needed
}

void RollState::Update(Player* _p, float _dt)
{
    _p->GetAnimations()->Update(EN_AN_ROLL, _dt, _p->GetSrcRect());
        int choice = rand() % 3; // 0=Rock, 1=Paper, 2=Scissors

        switch (choice) {
        case 0: _p->SetState(PlayerState::GetRockState()); break;
        case 1: _p->SetState(PlayerState::GetPaperState()); break;
        case 2: _p->SetState(PlayerState::GetScissorState()); break;
        }
}

void RockState::HandleInput(Player* _p, SDL_Event _e)
{
    if (InputController::Instance().KB()->KeyDown(_e, SDLK_SPACE)) {
        _p->SetState(PlayerState::GetRollState());
    }
    else
    {
        _p->SetState(PlayerState::GetNormState());
    }
}

void RockState::Update(Player* _p, float _dt)
{
    _p->GetAnimations()->Update(EN_AN_ROCK, _dt, _p->GetSrcRect());
}

void PaperState::HandleInput(Player* _p, SDL_Event _e)
{
    if (InputController::Instance().KB()->KeyDown(_e, SDLK_SPACE)) {
        _p->SetState(PlayerState::GetRollState());
    }
    else
    {
        _p->SetState(PlayerState::GetNormState());
    }
}

void PaperState::Update(Player* _p, float _dt)
{
    _p->GetAnimations()->Update(EN_AN_PAPER, _dt, _p->GetSrcRect());
}

void ScissorState::HandleInput(Player* _p, SDL_Event _e)
{
    if (InputController::Instance().KB()->KeyDown(_e, SDLK_SPACE)) {
        _p->SetState(PlayerState::GetRollState());
    }
    else
    {
        _p->SetState(PlayerState::GetNormState());
    }
}

void ScissorState::Update(Player* _p, float _dt)
{
    _p->GetAnimations()->Update(EN_AN_SCISSOR, _dt, _p->GetSrcRect());
}

