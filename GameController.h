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

class Player;

class GameController : public Singleton<GameController> {

public:
    GameController();
    ~GameController();

    void Initialize();
    void ShutDown();
    void HandleInput(SDL_Event _event);
    void RunGame();

private:
    bool m_quit;
    SDL_Event m_sdlEvent;

    Renderer* m_renderer;
    TTFont* m_fArial20;
    InputController* m_input;
    PhysicsController* m_physics;
    Timing* m_timing;

    Level* m_currentLevel;
    Level* m_nextLevel; 

    Level* level1 = nullptr;
    Level* level2 = nullptr;

    Player* player1;

    glm::vec2 m_spawnAreaMin;
    glm::vec2 m_spawnAreaMax;
    glm::vec2 m_destinationAreaMin;
    glm::vec2 m_destinationAreaMax;

    std::vector<Unit*> m_units;
    Texture* m_texture = nullptr;
};