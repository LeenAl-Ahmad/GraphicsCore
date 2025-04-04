#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#include "Level.h"
#include "Renderer.h"
#include "Timing.h"
#include "TTFont.h"

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
    Level* m_currentLevel = nullptr;
    Timing* timing;
    InputController* m_input;
    PhysicsController* m_physics;
    bool m_quit;
    TTFont* m_fpsFont;
    float m_currentFPS;

    std::map<RigidBody*, Unit*> m_physicsToUnitMap;
};

#endif