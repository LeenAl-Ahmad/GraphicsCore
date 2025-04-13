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
    m_audio = nullptr;
    m_sound = nullptr;
    m_song = nullptr;
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

    m_nickname1 = "";
    m_nickname2 = "";
    m_showError = false;

    // Initialize pools
    Texture::Pool = new ObjectPool<Texture>();
    SpriteSheet::Pool = new ObjectPool<SpriteSheet>();
    SpriteAnim::Pool = new ObjectPool<SpriteAnim>();

    // Load texture
    m_texture = Texture::Pool->GetResource();
    m_texture->Load("./Assets/Textures/ackground.tga");

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
    // Handle quit command
    if ((_event.type == SDL_QUIT)) {
        m_quit = true;
        return;
    }

    // Level 1 nickname handling
    if (m_currentLevel == level1) {
        string input = m_input->KB()->TextInput(_event);

        // Add input to current nickname
        if (!input.empty()) {
            if (!m_acceptingNickname2) {
                m_nickname1 += input;  // Always add to nickname1 first
            }
            else {
                m_nickname2 += input;  // Switch to nickname2 when flag is set
            }
        }

        // Press 'A' to start assigning to nickname2
        if (m_input->KB()->KeyDown(_event, SDLK_RETURN)) {
            m_acceptingNickname2 = true;
        }
    }

    // Player controls
    player1->HandleInput(_event, m_timing->GetDeltaTime());
    m_input->MS()->ProcessButtons(_event);
}
 
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
        SDL_Color{ 255, 255, 0},
        SDL_Point{ 400, 400 });

    // Yellow nickname displays
    m_fArial20->Write(m_renderer->GetRenderer(),
        "Player 1:",
        SDL_Color{ 255, 255, 0, 255 },
        SDL_Point{ 40, 80 });

    m_fArial20->Write(m_renderer->GetRenderer(),
        m_nickname1.c_str(),
        SDL_Color{ 255, 255, 0},
        SDL_Point{ 160, 80 });

    m_fArial20->Write(m_renderer->GetRenderer(),
        "Player 2:",
        SDL_Color{ 255, 255, 0, 255 },
        SDL_Point{ 40, 220 });

    m_fArial20->Write(m_renderer->GetRenderer(),
        m_nickname2.c_str(),
        SDL_Color{ 255, 255, 0, 255 },
        SDL_Point{ 80, 220 });

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
            
            m_renderer->ClearScreen();
            
            m_physics->Update(m_timing->GetDeltaTime());
            if (player1) {
                player1->Update(m_timing->GetDeltaTime());
                player1->Render(m_renderer);
                player1->GetRigidBody()->SetPosition({ 20, 30 });
                m_fArial20->Write(m_renderer->GetRenderer(),
                    "Player 1", SDL_Color{ 0,255,0 }, SDL_Point{ 20, 20 });
                m_fArial20->Write(m_renderer->GetRenderer(),
                    "Wins: ", SDL_Color{ 0,0,255 }, SDL_Point{ 20, 40 });
                m_fArial20->Write(m_renderer->GetRenderer(),
                    "Losses: ", SDL_Color{ 0,0,255 }, SDL_Point{ 20, 60 });
            }
            if (player2)
            { }
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



