#include "GameController.h"
#include "Player.h"

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
    player1 = nullptr;
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
    player1 = new Player();
    m_currentLevel = new Level(m_physics);
}

void GameController::ShutDown() {
    if (player1 != nullptr)
    {
        delete player1;
        player1 = nullptr;
    }
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
    
    player1->HandleInput(_event, m_timing->GetDeltaTime());
    m_input->MS()->ProcessButtons(_event);
}

void GameController::RunGame() {
    Initialize();

    while (!m_quit) {
        m_timing->Tick();
        m_renderer->SetDrawColor(Color(255, 255, 255, 255));
        m_renderer->ClearScreen();

        // Input handling
        while (SDL_PollEvent(&m_sdlEvent) != 0) {
            HandleInput(m_sdlEvent);
        }

        // Update systems
        m_physics->Update(m_timing->GetDeltaTime());
        player1->Update(m_timing->GetDeltaTime());
        
        player1->Render(m_renderer);

        /*m_fArial20->Write(m_renderer->GetRenderer(),
            ("FPS: " + to_string(m_timing->GetFPS())).c_str(),
            SDL_Color{ 0, 0, 255 }, SDL_Point{ 10, 10 });

        m_fArial20->Write(m_renderer->GetRenderer(),
            ("Units: " + to_string(m_currentLevel->GetUnitCount())).c_str(),  // Add this method to Level
            SDL_Color{ 0, 0, 255 }, SDL_Point{ 10, 40 });

        m_fArial20->Write(m_renderer->GetRenderer(),
            m_physics->ToString().c_str(),
            SDL_Color{ 0, 0, 255 }, SDL_Point{ 120, 10 });*/
        

        SDL_RenderPresent(m_renderer->GetRenderer());
    }

    ShutDown();
}