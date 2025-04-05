#include "Level.h"

Level::Level()
{
    physics = nullptr;
    player = nullptr;
}

Level::~Level() {
    if (player) {
        Unit::Pool->ReturnResource(player);
    }
}

void Level::AssignNonDefaultValues() {
    physics = &PhysicsController::Instance();
    Unit::Pool = new ObjectPool<Unit>();
    SpriteSheet::Pool = new ObjectPool<SpriteSheet>();
    SpriteAnim::Pool = new ObjectPool<SpriteAnim>();

    player = Unit::Pool->GetResource();
    player->AssignNonDefaultValues();

    // Start player in center of screen (assuming 1920x1080 resolution)
    player->SetPosition(glm::vec2(1920 / 2, 1080 / 2));
}
void Level::HandleInput(SDL_Event event)
{
    if (player) {
        player->HandleInput(event);
    }
}
void Level::Update(float deltaTime) {
    if (player) {
        player->UpdateMovement(deltaTime);
    }
    physics->Update(deltaTime);
}

void Level::Render(Renderer* renderer, Timing* timing) {
    renderer->SetDrawColor(Color(255, 255, 255, 255));
    renderer->ClearScreen();

    if (player) {
        // Get the current animation frame
        Rect srcRect = player->Update(player->IsMoving() ? EN_AN_RUN : EN_AN_IDLE,
            timing->GetDeltaTime());

        // Calculate destination rectangle
        unsigned int width = static_cast<unsigned int>(69 * player->GetScale());
        unsigned int height = static_cast<unsigned int>(44 * player->GetScale());
        unsigned int x = static_cast<unsigned int>(player->GetPosition().x);
        unsigned int y = static_cast<unsigned int>(player->GetPosition().y);

        Rect destRect(x, y, x + width, y + height);

        // Get the SDL_Texture from the warrior's sprite sheet
        SDL_Texture* warriorTexture = renderer->GetSDLTexture(player->GetWarrior());

        // Render with rotation
        renderer->RenderTexture(warriorTexture,
            srcRect,
            destRect,
            (0,0));
    }
}