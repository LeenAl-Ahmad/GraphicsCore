#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#include "StandardIncludes.h"
#include "Level.h"

class GameController : public Singleton<GameController> {
public:
    GameController();
    virtual ~GameController();

    void RunGame();

private:
    SDL_Event m_sdlEvent;
    Level* currentLevel;
};

#endif // GAME_CONTROLLER_H
