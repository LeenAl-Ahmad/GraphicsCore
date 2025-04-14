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
class Level;
class Player;
class Texture;

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

    //for Level
    void RenderLevel1UI();
private:
    // Members 
    bool m_quit;
    SDL_Event m_sdlEvent;
    Renderer* m_renderer;
    TTFont* m_fArial20;
    InputController* m_input;
    PhysicsController* m_ph;
    Timing* m_time;
    Level* level1;
    Player* player1;
    Texture* m_texture;

    bool m_showError;
    string assign ="";
    bool m_acceptingNickname1 = false;
    bool m_acceptingNickname2 = false;
    bool currentlevel = true;
    std::string m_nickname1;
    std::string m_nickname2;
    glm::vec2 m_spawnAreaMin;
    glm::vec2 m_spawnAreaMax;
    glm::vec2 m_destinationAreaMin;
    glm::vec2 m_destinationAreaMax;
    int pressedReturn = 0;

    //Sound
    AudioController* m_audio;
    SoundEffect* m_effects[MaxEffectChannels];
    Song* m_song;
};

#endif // GAME_CONTROLLER_H
