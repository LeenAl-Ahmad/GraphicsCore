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
{
    m_quit = false;
    m_sdlEvent = {};
    m_renderer = nullptr;
    m_fArial20 = nullptr;
    m_input = nullptr;
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
    AssetController::Instance().Initialize(10000000);
    m_renderer = &Renderer::Instance();
    m_renderer->Initialize();
    m_input = &InputController::Instance();
    m_fArial20 = new TTFont();
    m_fArial20->Initialize(20);
    m_audio = &AudioController::Instance();
    m_effects[0] = m_audio->LoadEffect("C:/Users/leana/source/repos/GraphicsCore/Assets/Audio/Effects/Whoosh.wav");
    m_effects[1] = m_audio->LoadEffect("C:/Users/leana/source/repos/GraphicsCore/Assets/Audio/Effects/BeeFlyingLoop.mp3");
    m_effects[2] = m_audio->LoadEffect("C:/Users/leana/source/repos/GraphicsCore/Assets/Audio/Effects/DistantGunshot.mp3");
    m_effects[3] = m_audio->LoadEffect("C:/Users/leana/source/repos/GraphicsCore/Assets/Audio/Effects/DrinkSipSwallow.mp3");
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
        m_audio->Play(m_effects[rand() % 4]);
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
        for (int count = 0; count < MaxEffectChannels; count++)
        {
            string eff = "Effect " + to_string(count) + ": ";
            eff += m_audio->GetCurrentEffects()[count];
            m_fArial20->Write(m_renderer->GetRenderer(), eff.c_str(), { 255, 0, 255 }, { 10, 30 + (count * 20) });
        }
        
        SDL_RenderPresent(m_renderer->GetRenderer());
    }
}

