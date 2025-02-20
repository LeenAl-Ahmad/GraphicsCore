#include "GameController.h"
#include "Renderer.h"
#include "TTFont.h"
#include "Timing.h"

GameController::GameController() {
    m_sdlEvent = { };
    currentLevel = new Level();
}

GameController::~GameController() {
    delete currentLevel;
}

void GameController::RunGame() {
    AssetController::Instance().Initialize(10000000); // Allocate 10MB
    Renderer* r = &Renderer::Instance();
    Timing* t = &Timing::Instance();
    r->Initialize(800, 600);

    // Ensure Object Pools are properly initialized
    SpriteSheet::Pool = new ObjectPool<SpriteSheet>();
    SpriteAnim::Pool = new ObjectPool<SpriteAnim>();

    // Initialize the level
    Level level;
    level.Initialize(r);

    while (m_sdlEvent.type != SDL_QUIT) {
        t->Tick();
        SDL_PollEvent(&m_sdlEvent);

        // Update and render the level
        level.Update(t->GetDeltaTime());
        level.Render(r, t);
    }

    // Cleanup
    delete SpriteAnim::Pool;
    delete SpriteSheet::Pool;
    r->Shutdown();
}

