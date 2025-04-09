#pragma once
#include "Renderer.h"
#include "TTFont.h"
#include "InputController.h"
#include "PhysicsController.h"
#include "Timing.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "SpriteSheet.h"
#include "SpriteAnim.h"
#include "RigidBody.h"
#include "Level.h"

class GameController : public Singleton<GameController> {
private:
    bool m_quit;
    SDL_Event m_sdlEvent;

    Renderer* m_renderer;
    TTFont* m_fArial20;
    InputController* m_input;
    PhysicsController* m_physics;
    Timing* m_timing;

    Level* m_currentLevel;

    SpriteSheet* m_fire;
    SpriteSheet* m_smoke;

public:
    GameController();
    ~GameController();

    void Initialize();
    void ShutDown();
    void HandleInput(SDL_Event _event);
    void RunGame();
};