#include "GameController.h"
#include "Renderer.h"
#include "TTFont.h"
#include "InputController.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Controller.h"
#include "AudioController.h"
#include "SoundEffect.h"
#include "Song.h"

GameController::GameController()
    : m_quit(false), m_sdlEvent({}), m_renderer(nullptr), m_fArial20(nullptr),
    m_input(nullptr), m_audio(nullptr), m_effect(nullptr), m_song(nullptr){}

GameController::~GameController()
{
    ShutDown();
}

void GameController::Initialize()
{
    AssetController::Instance().Initialize(10000000);
    m_renderer = &Renderer::Instance();
    m_renderer->Initialize();
    m_input = &InputController::Instance();
    m_fArial20 = new TTFont();
    m_fArial20->Initialize(20);
    m_audio = &AudioController::Instance();
    m_effect = m_audio->LoadEffect("C:/Users/leana/source/repos/GraphicsCore/Assets/Audio/Effects/Whoosh.wav");
    m_song = m_audio->LoadSong("C:/Users/leana/source/repos/GraphicsCore/Assets/Audio/Music/Track1.mp3");
}

void GameController::HandleInput(SDL_Event _event)
{
    if ((_event.type == SDL_QUIT) ||
        (m_input->KB()->KeyUp(_event, SDLK_ESCAPE)))
    {
        m_quit = true;
    }
    else if (m_input->KB()->KeyUp(_event, SDLK_p))
    {
        m_audio->Play(m_effect);
    }
    else if (m_input->KB()->KeyUp(_event, SDLK_a))
    {
        m_audio->Play(m_song);
    }
    else if (m_input->KB()->KeyUp(_event, SDLK_s))
    {
        m_audio->PauseMusic();
    }
    else if (m_input->KB()->KeyUp(_event, SDLK_d))
    {
        m_audio->ResumeMusic();
    }
    else if (m_input->KB()->KeyUp(_event, SDLK_f))
    {
        m_audio->StopMusic();
    }

    else m_input->MS()->ProcessButtons(_event);
}

void GameController::ShutDown()
{
    delete m_fArial20;
}

void GameController::RunGame()
{
    Initialize();

    while (!m_quit)
    {
        m_renderer->SetDrawColor(Color(255, 255, 255, 255));
        m_renderer->ClearScreen();

        while (SDL_PollEvent(&m_sdlEvent) != 0)
        {
            HandleInput(m_sdlEvent);
        }

        string song = "Current Song: " + m_audio->GetMusicTitle();
        if (m_audio->GetMusicLength() != "")
        {
            song += " " + to_string((int)m_audio->MusicPosition()) + "/" + m_audio->GetMusicLength();
        }
        m_fArial20->Write(m_renderer->GetRenderer(), song.c_str(), { 0,0,255 }, { 10,10 });
        m_fArial20->Write(m_renderer->GetRenderer(), ("Current Effect: " + m_audio->GetCurrentEffect()).c_str(), { 0,0,255 }, { 10, 30 });

        SDL_RenderPresent(m_renderer->GetRenderer());
    }
    /*AssetController::Instance().Initialize(10000000); // Allocate 10MB
    Renderer* r = &Renderer::Instance();
    Timing* t = &Timing::Instance();
    r->Initialize();
    r->EnumerateDisplayMode();
    r->ChangeDisplayMode(&r->GetResolutions()[8]);

    TTFont* font = new TTFont();
    font->Initialize(20);

    // Initialize pools
    SpriteSheet::Pool = new ObjectPool<SpriteSheet>();
    SpriteAnim::Pool = new ObjectPool<SpriteAnim>();
    SpriteSheet* sheet = SpriteSheet::Pool->GetResource();
    sheet->Load("./Assets/Textures/Warrior.tga");
    sheet->SetSize(17, 6, 69, 44);
    sheet->AddAnimation(EN_AN_IDLE, 0, 6, 6.0f);
    sheet->AddAnimation(EN_AN_RUN, 6, 8, 6.0f);

    RenderTarget* rt = new RenderTarget;
    rt->Create(NATIVE_XRES, NATIVE_YRES);

    // Main game loop
    while (m_sdlEvent.type != SDL_QUIT)
    {
        t->Tick();
        rt->Start();
        r->SetDrawColor(Color(255, 255, 255, 255));
        r->ClearScreen();
        r->RenderTexture(sheet, sheet->Update(EN_AN_RUN, t->GetDeltaTime()), Rect(0, 150, 69 * 3, 150 + 44 * 3));

        std::string fps = "Frames Per Second: " + std::to_string(t->GetFPS());
        font->Write(r->GetRenderer(), fps.c_str(), SDL_Color{ 0, 0, 255 }, SDL_Point{ 0, 0 });
        std::string s = "Frame number: " + std::to_string(sheet->GetCurrentClip(EN_AN_RUN));
        font->Write(r->GetRenderer(), s.c_str(), SDL_Color{ 0, 255, 0 }, SDL_Point{ 250, 200 });

        rt->stop();
        r->SetDrawColor(Color(0, 0, 0, 255));
        r->ClearScreen();
        rt->Render(t->GetDeltaTime());
        SDL_RenderPresent(r->GetRenderer()); 
        t->CapFPS();
    }

    // Clean up
    delete rt;
    delete SpriteAnim::Pool;
    delete SpriteSheet::Pool;
    font->Shutdown();
    r->Shutdown();*/
}

