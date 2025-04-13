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

#include "AudioController.h"
#include "SoundEffect.h"
#include "Song.h"

class Player;

class GameController : public Singleton<GameController> {

public:
    GameController();
    ~GameController();

    void Initialize();
    void ShutDown();
    void HandleInput(SDL_Event _event);
    void RunGame();

    void RenderLevel1UI();

    std::string SetName1(std::string input) { return m_nickname1 = input; }

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
    Player* player2;


    std::vector<Unit*> m_units;
    Texture* m_texture = nullptr;

    std::string m_nickname1;
    std::string m_nickname2;
    bool m_acceptingNickname1 = false;
    bool m_acceptingNickname2 = false;
    bool m_showError;

    std::string assign;
    AudioController* m_audio;
    SoundEffect* m_sound;
    Song* m_song;
};