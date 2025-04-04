#include "GameController.h"

GameController::GameController() {
    m_renderer = &Renderer::Instance();
    m_input = nullptr;
    m_quit = false;
    m_physics = nullptr;
    m_fpsFont = nullptr;
}

GameController::~GameController() {
    ShutDown();
}

void GameController::Initialize() {
    // Initialize renderer
    m_renderer->Initialize();

    m_input = &InputController::Instance();
    m_physics = &PhysicsController::Instance();
    m_fpsFont = new TTFont();
    m_fpsFont->Initialize(20);
    m_currentFPS = 0.0f;
    timing = &Timing::Instance();
    // Create initial level
    m_currentLevel = new Level();
    m_currentLevel->AssignNonDefaultValues();
}

void GameController::ShutDown() {
    delete m_fpsFont;

    if (m_currentLevel) {
        delete m_currentLevel;
        m_currentLevel = nullptr;
    }
}

void GameController::HandleInput(SDL_Event event) {
    // 1. Safety checks
    if (!m_input || !m_input->KB() || !m_input->MS() || !m_physics || !m_currentLevel) {
        return;
    }

    // 2. Quit handling
    if (event.type == SDL_QUIT || m_input->KB()->KeyUp(event, SDLK_ESCAPE)) {
        m_quit = true;
        m_currentLevel->ShouldQuit();
        return;
    }

    // 3. Unit spawning
    if (m_input->KB()->KeyDown(event, SDLK_a)) {
        // Calculate random position/direction
        glm::vec2 pos(16 + rand() % (1920 - 32), 16 + rand() % (1080 - 32));
        glm::vec2 dir = glm::normalize(
            glm::vec2(rand() % 1920, rand() % 1080) - pos
        ) * 200.0f;

        // Create visual unit
        Unit* unit = m_currentLevel->CreateUnit(pos);

        // Create physics body
        RigidBody* body = m_physics->AddRigidBody(pos, dir, rand() % 128);

        // Link them in the map
        if (unit && body) {
            m_physicsToUnitMap[body] = unit;
        }
    }

    // 4. Mouse handling
    m_input->MS()->ProcessButtons(event);
}

void GameController::SyncPhysicsToUnits() {
    for (auto& pair : m_physicsToUnitMap) {
        RigidBody* body = pair.first;
        Unit* unit = pair.second;

        if (body && unit) {
            // Access position/velocity directly from RigidBody
            unit->position = body->GetPosition();  
            unit->velocity = body->GetVelocity();  
        }
    }
}

void GameController::RunGame() {
    Initialize();

    SDL_Event event;
    while (!m_quit) {
        timing->Tick();

        m_renderer->SetDrawColor(Color(255, 255, 255, 255));
        m_renderer->ClearScreen();

        // Input
        while (SDL_PollEvent(&event)) {
            HandleInput(event);
        }

        // Update
        m_currentLevel->Update(timing->GetDeltaTime());
        SyncPhysicsToUnits();  // Critical sync step

        // Render
        m_currentLevel->Render(m_renderer);

        m_fpsFont->Write(m_renderer->GetRenderer(), ("FPS: " + to_string(timing->GetFPS())).c_str(), SDL_Color{ 1,1,255 }, SDL_Point{ 10,10 });
        SDL_RenderPresent(m_renderer->GetRenderer());


        timing->CapFPS();
    }

    ShutDown();
}
