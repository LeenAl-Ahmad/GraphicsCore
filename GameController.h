#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H


#include "Renderer.h"
#include "Timing.h"
#include "TTFont.h"
#include "InputController.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "PhysicsController.h"
#include "Unito.h"

class Level;

class GameController : public Singleton<GameController>
{
public:
    GameController();
    ~GameController();

    void Initialize();
    void RunGame();
    void ShutDown();
    void HandleInput(SDL_Event _event);

    void SyncPhysicsToUnits();

private:
    Renderer* m_renderer = nullptr;
    Timing* timing;
    InputController* m_input;
    PhysicsController* m_physics;
    bool m_quit;
    TTFont* m_fpsFont;
    float m_currentFPS;
    SDL_Event m_sdlEvent;

    Level* level;

    Unit* warrior;
    SpriteSheet* warriors;
    std::map<RigidBody*, Unit*> m_physicsToUnitMap;
};

#endif