#include "GameController.h"
#include "Renderer.h"
#include "TTFont.h"
#include "InputController.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "SoundEffect.h"
#include "WavDraw.h"
#include "PhysicsController.h"
#include "Timing.h"
#include "Level.h"
#include "Player.h"
#include "Texture.h"


GameController::GameController()
{
    
    m_quit = false;
    m_sdlEvent = {};
    m_renderer = nullptr;
    m_fArial20 = nullptr;
    m_input = nullptr;
    m_ph = nullptr;
    m_time = nullptr;
    level1 = nullptr;
    player1 = nullptr;
    m_texture = nullptr;
    m_nickname1 = assign;

    m_audio = nullptr;
    memset(m_effects, 0, sizeof(SoundEffect*) * MaxEffectChannels);
    m_song = nullptr;
}

GameController::~GameController()
{
    ShutDown();
}

void GameController::Initialize()
{
    //check
    AssetController::Instance().Initialize(10000000);
    m_renderer = &Renderer::Instance();
    m_renderer->Initialize();
    m_input = &InputController::Instance();
    m_fArial20 = new TTFont();
    m_fArial20->Initialize(20);
    m_time = &Timing::Instance();
    m_ph = &PhysicsController::Instance();
    m_audio = &AudioController::Instance();
    
    Texture::Pool = new ObjectPool<Texture>();
    SpriteSheet::Pool = new ObjectPool<SpriteSheet>();
    SpriteAnim::Pool = new ObjectPool<SpriteAnim>();

    m_texture = Texture::Pool->GetResource();
    m_texture->Load("C:/Users/leana/source/repos/GraphicsCore/Assets/Textures/ackground.tga");
    m_effects[0] = m_audio->LoadEffect("C:/Users/leana/source/repos/GraphicsCore/Assets/Audio/Effects/Whoosh.wav");
    m_song = m_audio->LoadSong("C:/Users/leana/source/repos/GraphicsCore/Assets/Audio/Music/Track1.mp3");
    if (currentlevel) 
    { m_audio->Play(m_song); }
    else
    {
       
    }
}

void GameController::ShutDown()
{
    if (m_fArial20) {
        delete m_fArial20;
        m_fArial20 = nullptr;
    }

    if (m_texture) {
        Texture::Pool->ReleaseResource(m_texture);
        m_texture = nullptr;
    }
    
    if (SpriteAnim::Pool) {
        delete SpriteAnim::Pool;
        SpriteAnim::Pool = nullptr;
    }

    if (SpriteSheet::Pool) {
        delete SpriteSheet::Pool;
        SpriteSheet::Pool = nullptr;
    }
    if (Texture::Pool) {
        delete Texture::Pool;
        Texture::Pool = nullptr;
    }
}

void GameController::HandleInput(SDL_Event _event)
{
    string temp;
    if ((_event.type == SDL_QUIT) ||
        (m_input->KB()->KeyUp(m_sdlEvent, SDLK_ESCAPE)))
    {
        m_quit = true;
    }

    else if ((temp = m_input->KB()->TextInput(_event)) != "")
    {
        assign += temp;
    }

    else if (m_input->KB()->KeyDown(m_sdlEvent, SDLK_BACKSPACE))
    {
        
    }
    
    else if (m_input->KB()->KeyUp(m_sdlEvent, SDLK_RETURN))
    {
        pressedReturn++;
        if (pressedReturn == 1)
        {
            m_nickname1 = assign;
            m_acceptingNickname1 = true;
        }
        else if (pressedReturn == 2)
        {
            m_nickname2 = assign;
            m_acceptingNickname2 = true;
        }
        assign = "";
        if (pressedReturn == 3)
        {
            m_audio->StopMusic();
            currentlevel = false;
        }
    }
    
    else m_input->MS()->ProcessButtons(_event);

}

void GameController::RunGame()
{
    Initialize();

    while (!m_quit)
    {
        m_time->Tick();
        
        if (currentlevel)
        {
            while (SDL_PollEvent(&m_sdlEvent) != 0)
            {
                HandleInput(m_sdlEvent);
            }
            m_renderer->RenderTexture(m_texture, Point(0, 0));
            RenderLevel1UI();
        }
        else
        {
            m_renderer->SetDrawColor(Color(255, 255, 255, 255));
            m_renderer->ClearScreen();
            
        }

        SDL_RenderPresent(m_renderer->GetRenderer());
    }
    ShutDown();
}


//for level
void GameController::RenderLevel1UI() {
    // White instructional text at bottom
    m_fArial20->Write(m_renderer->GetRenderer(),
        "Quit [ESC]: Quits the application",
        SDL_Color{ 255, 255, 255, 255 },
        SDL_Point{ 50, 1000 });

    m_fArial20->Write(m_renderer->GetRenderer(),
        "Next Nickname [Return]: Continue to next nickname",
        SDL_Color{ 255, 255, 255, 255 },
        SDL_Point{ 50, 1030 });

    m_fArial20->Write(m_renderer->GetRenderer(),
        assign.c_str(),
        SDL_Color{ 255, 255, 0 },
        SDL_Point{ 400, 400 });

    // Yellow nickname displays
    m_fArial20->Write(m_renderer->GetRenderer(),
        "Player 1:",
        SDL_Color{ 255, 255, 0, 255 },
        SDL_Point{ 40, 80 });

    m_fArial20->Write(m_renderer->GetRenderer(),
        m_nickname1.c_str(),
        SDL_Color{ 255, 255, 0 },
        SDL_Point{ 160, 80 });

    m_fArial20->Write(m_renderer->GetRenderer(),
        "Player 2:",
        SDL_Color{ 255, 255, 0, 255 },
        SDL_Point{ 40, 220 });

    m_fArial20->Write(m_renderer->GetRenderer(),
        m_nickname2.c_str(),
        SDL_Color{ 255, 255, 0, 255 },
        SDL_Point{ 160, 220 });

    // Show which nickname is being entered
    if (m_acceptingNickname1 == false) {
        m_fArial20->Write(m_renderer->GetRenderer(),
            "->",
            SDL_Color{ 0, 255, 0, 255 },
            SDL_Point{ 20, 80 });
    }
    else if (m_acceptingNickname1 == true) {
        m_fArial20->Write(m_renderer->GetRenderer(),
            "->",
            SDL_Color{ 0, 255, 0, 255 },
            SDL_Point{ 20, 220 });
    }

    // Error message if needed
    if (m_showError) {
        m_fArial20->Write(m_renderer->GetRenderer(),
            "ERROR: Nickname cannot be empty!",
            SDL_Color{ 255, 0, 0, 255 },
            SDL_Point{ 400, 500 });
    }
}