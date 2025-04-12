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
    m_currentLevel = level1;
    m_nextLevel = level2;
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
    
    m_spawnAreaMin = glm::vec2(16, 16);
    m_spawnAreaMax = glm::vec2(1920 - 32, 1080 - 32);
    m_destinationAreaMin = glm::vec2(0, 0);
    m_destinationAreaMax = glm::vec2(1920, 1080);

    // Initialize pools
    Texture::Pool = new ObjectPool<Texture>();
    SpriteSheet::Pool = new ObjectPool<SpriteSheet>();
    SpriteAnim::Pool = new ObjectPool<SpriteAnim>();

    // Load texture
    m_texture = Texture::Pool->GetResource();
    m_texture->Load("C:/Users/leana/source/repos/GraphicsCore/Assets/Textures/ackground.tga");

    // Create player and levels
    player1 = new Player();
    level1 = new Level(m_physics);
    level2 = new Level(m_physics);

    // Add player to initial level
    m_currentLevel = level1;
    
}

void GameController::ShutDown() {
    // Release resources in reverse order of creation
    if (m_texture) {
        Texture::Pool->ReleaseResource(m_texture);
        m_texture = nullptr;
    }

    if (player1) {
        delete player1;
        player1 = nullptr;
    }

    if (level1) {
        delete level1;
        level1 = nullptr;
    }

    if (level2) {
        delete level2;
        level2 = nullptr;
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

    if (Texture::Pool) {
        delete Texture::Pool;
        Texture::Pool = nullptr;
    }
}

void GameController::HandleInput(SDL_Event _event) {
    if ((_event.type == SDL_QUIT) ||
        (m_input->KB()->KeyUp(_event, SDLK_ESCAPE))) {
        m_quit = true;
    }

    if (m_input->KB()->KeyDown(_event, SDLK_n))
    {
        m_currentLevel = level2;
    }
    
    player1->HandleInput(_event, m_timing->GetDeltaTime());
    m_input->MS()->ProcessButtons(_event);
}

void GameController::RunGame() {
    Initialize();

    while (!m_quit) {
        m_timing->Tick();
        

        // Input handling
        while (SDL_PollEvent(&m_sdlEvent) != 0) {
            HandleInput(m_sdlEvent);
        }

        // Clear screen
        m_renderer->SetDrawColor(Color(255, 255, 255, 255)); // White
        m_renderer->ClearScreen();

        // Level-specific rendering
        if (m_currentLevel == level1) {
            m_renderer->RenderTexture(m_texture, Point(0, 0));
        }
        else
        {
            m_physics->Update(m_timing->GetDeltaTime());
            if (player1) {
                player1->Update(m_timing->GetDeltaTime());
                player1->Render(m_renderer);
            }
        }
        

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