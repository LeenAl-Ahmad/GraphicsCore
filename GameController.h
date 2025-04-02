#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#include "StandardIncludes.h"
#include "AudioController.h"

class Renderer;
class TTFont;
class InputController;
class WavDraw;
class SoundEffect;
class Song;
class PhysicsController;
class Timing;
class SpriteSheet;

class GameController : public Singleton<GameController>
{
public:
    // Constructors/Destructors
    GameController();
    virtual ~GameController();

    // Methods
    void RunGame();
    void Initialize();
    void HandleInput(SDL_Event _event);
    void ShutDown();

private:
    // Members
    SDL_Event m_sdlEvent;
    Renderer* m_renderer;
    TTFont* m_fArial20;
    bool m_quit;
    InputController* m_input;
    AudioController* m_audio;
    WavDraw* m_wavDraw;
    SoundEffect* m_effects[MaxEffectChannels];
    Song* m_song;
    float m_zoomY;
    Timing* m_timing;
    PhysicsController* m_physics;
    SpriteSheet* m_fire;
    SpriteSheet* m_smoke;
    SpriteSheet* m_circle;

    int m_windowWidth;
    int m_windowHeight;
};

#endif // GAME_CONTROLLER_H
