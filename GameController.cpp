// GameController.cpp
#include "GameController.h"
#include "Renderer.h"
#include "TTFont.h"
#include "Timing.h"

GameController::GameController() {
    m_sdlEvent = { };
    currentLevel = new Level();
    nextLevel = nullptr; 
}

GameController::~GameController() {
    delete currentLevel;
    if (nextLevel) {
        delete nextLevel;
    }
}

void GameController::RunGame() {
    AssetController::Instance().Initialize(10000000); // Allocate 10MB
    Renderer* r = &Renderer::Instance();
    Timing* t = &Timing::Instance();
    r->Initialize(1920, 1080);

    // Ensure Object Pools are properly initialized
    SpriteSheet::Pool = new ObjectPool<SpriteSheet>();
    SpriteAnim::Pool = new ObjectPool<SpriteAnim>();

    // Initialize the first level
    currentLevel = new Level(1);
    currentLevel->Initialize(r);

    while (m_sdlEvent.type != SDL_QUIT) {
        t->Tick();
        SDL_PollEvent(&m_sdlEvent);

        // Update and render the current level
        currentLevel->Update(t->GetDeltaTime());
        currentLevel->Render(r, t);

        // Check if the current level is complete
        if (currentLevel->IsComplete()) {
            // Load Level 2
            if (!nextLevel) {
                nextLevel = new Level(2);
                nextLevel->Initialize(r);
            }

            // Switch to the next level
            delete currentLevel;
            currentLevel = nextLevel;
            nextLevel = nullptr;
        }
    }

    // Cleanup
    delete SpriteAnim::Pool;
    delete SpriteSheet::Pool;
    r->Shutdown();
}