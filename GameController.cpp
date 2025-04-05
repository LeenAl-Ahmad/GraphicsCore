#include "GameController.h"
#include "Level.h"

GameController::GameController() {
    m_input = nullptr;
    m_quit = false;
    m_physics = nullptr;
    m_fpsFont = nullptr;
    warrior = nullptr;
}

GameController::~GameController() {
    ShutDown();
}

void GameController::Initialize() {
    // Initialize renderer
    AssetController::Instance().Initialize(10000000);
    
    m_renderer = &Renderer::Instance();
    m_renderer->Initialize();

    m_input = &InputController::Instance();
    m_physics = &PhysicsController::Instance();
    m_fpsFont = new TTFont();
    m_fpsFont->Initialize(20);
    m_currentFPS = 0.0f;
    timing = &Timing::Instance();
    timing->SetFPS(80);
    level = new Level();
    level->AssignNonDefaultValues();
}

void GameController::ShutDown() {
    delete m_fpsFont;

}

void GameController::RunGame() {
    Initialize();

    while (!m_quit) {
        timing->Tick();

        // Input - process all pending events
        while (SDL_PollEvent(&m_sdlEvent) != 0) {
            if (m_sdlEvent.type == SDL_QUIT) {
                m_quit = true;
            }
            // Let level handle other input events
            level->HandleInput(m_sdlEvent);
        }

        // Update game state every frame (not just when events occur)
        level->Update(timing->GetDeltaTime());

        // Render
        level->Render(m_renderer, timing);

        // UI Overlays
        m_fpsFont->Write(m_renderer->GetRenderer(),
            ("FPS: " + to_string(timing->GetFPS())).c_str(),
            SDL_Color{ 0,0,255 },
            SDL_Point{ 10,10 });
        m_fpsFont->Write(m_renderer->GetRenderer(),
            m_physics->ToString().c_str(),
            SDL_Color{ 0,0,255 },
            SDL_Point{ 120,10 });

        // Present
        SDL_RenderPresent(m_renderer->GetRenderer());

        // Frame rate control
        timing->CapFPS();
    }

    ShutDown();
}
