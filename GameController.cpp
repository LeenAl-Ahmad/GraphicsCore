#include "GameController.h"

// GameController Implementation
GameController::GameController() {
    m_quit = false;
    m_sdlEvent = {};
    m_renderer = nullptr;
    m_fArial20 = nullptr;
    m_input = nullptr;
    m_physics = nullptr;
    m_timing = nullptr;
    m_currentLevel = nullptr;
    m_fire = nullptr;
    m_smoke = nullptr;
}

GameController::~GameController() {
    ShutDown();
}

void GameController::Initialize() {
    AssetController::Instance().Initialize(10000000); // Allocate 10MB
    m_renderer = &Renderer::Instance();
    m_renderer->Initialize();
    m_input = &InputController::Instance();
    m_fArial20 = new TTFont();
    m_fArial20->Initialize(20);
    m_timing = &Timing::Instance();
    m_physics = &PhysicsController::Instance();

    SpriteSheet::Pool = new ObjectPool<SpriteSheet>();
    SpriteAnim::Pool = new ObjectPool<SpriteAnim>();

    m_fire = SpriteSheet::Pool->GetResource();
    m_fire->Load("C:/Users/leana/source/repos/GraphicsCore/Assets/Textures/Fire.tga");
    m_fire->SetSize(6, 10, 64, 64);
    m_fire->AddAnimation(EN_AN_IDLE, 0, 6, 20.0f);
    m_fire->SetBlendMode(SDL_BLENDMODE_BLEND);

    m_smoke = SpriteSheet::Pool->GetResource();
    m_smoke->Load("C:/Users/leana/source/repos/GraphicsCore/Assets/Textures/Smoke.tga");
    m_smoke->SetSize(5, 6, 128, 128);
    m_smoke->AddAnimation(EN_AN_SMOKE_RISE, 0, 30, 20.0f);
    m_smoke->SetBlendMode(SDL_BLENDMODE_BLEND);

    m_currentLevel = new Level(m_physics);
}

void GameController::ShutDown() {
    if (m_currentLevel) {
        delete m_currentLevel;
        m_currentLevel = nullptr;
    }

    if (m_fArial20) {
        delete m_fArial20;
        m_fArial20 = nullptr;
    }

    if (SpriteAnim::Pool) {
        delete SpriteAnim::Pool;
        SpriteAnim::Pool = nullptr;
    }

    if (SpriteSheet::Pool) {
        delete SpriteSheet::Pool;
        SpriteSheet::Pool = nullptr;
    }
}

void GameController::HandleInput(SDL_Event _event) {
    if ((_event.type == SDL_QUIT) ||
        (m_input->KB()->KeyUp(_event, SDLK_ESCAPE))) {
        m_quit = true;
    }
    else if (m_input->KB()->KeyDown(_event, SDLK_a)) {
        m_currentLevel->AddPhysicsObject();
    }
    m_input->MS()->ProcessButtons(_event);
}

void GameController::RunGame() {
    Initialize();

    while (!m_quit) {
        m_timing->Tick();
        m_renderer->SetDrawColor(Color(255, 255, 255, 255));
        m_renderer->ClearScreen();

        while (SDL_PollEvent(&m_sdlEvent) != 0) {
            HandleInput(m_sdlEvent);
        }

        m_physics->Update(m_timing->GetDeltaTime());
        m_currentLevel->Render(m_renderer, m_timing);

        m_fArial20->Write(m_renderer->GetRenderer(),
            ("FPS: " + to_string(m_timing->GetFPS())).c_str(),
            SDL_Color{ 0, 0, 255 }, SDL_Point{ 10, 10 });

        m_fArial20->Write(m_renderer->GetRenderer(),
            m_physics->ToString().c_str(),
            SDL_Color{ 0, 0, 255 }, SDL_Point{ 120, 10 });

        SDL_RenderPresent(m_renderer->GetRenderer());
    }

    ShutDown();
}