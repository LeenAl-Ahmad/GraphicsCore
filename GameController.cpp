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
    r->Initialize(800, 600);

    // Ensure Object Pools are properly initialized
    SpriteSheet::Pool = new ObjectPool<SpriteSheet>();
    SpriteAnim::Pool = new ObjectPool<SpriteAnim>();

    // Initialize the first level
    currentLevel->Initialize(r);

    while (m_sdlEvent.type != SDL_QUIT) {
        t->Tick();
        SDL_PollEvent(&m_sdlEvent);

        // Update and render the current level
        currentLevel->Update(t->GetDeltaTime());
        currentLevel->Render(r, t);

        // Check if the current level is complete
        if (currentLevel->IsComplete()) {
            // Save the current level
            currentLevel->Save("Level1.bin");

            // Load the next level
            if (!nextLevel) {
                nextLevel = new Level();
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