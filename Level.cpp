#include "Level.h"

Level::Level()
{
    physics = nullptr;
    player = nullptr;
    input = nullptr;
}

Level::~Level() {
    for (Unit* npc : npcWarriors) {
        Unit::Pool->ReturnResource(npc);
    }
    npcWarriors.clear();

    if (player) {
        Unit::Pool->ReturnResource(player);
    }
}

void Level::AssignNonDefaultValues() {
    input = &InputController::Instance();
    physics = &PhysicsController::Instance();
    Unit::Pool = new ObjectPool<Unit>();
    SpriteSheet::Pool = new ObjectPool<SpriteSheet>();
    SpriteAnim::Pool = new ObjectPool<SpriteAnim>();

    player = Unit::Pool->GetResource();
    player->AssignNonDefValues();

    // Start player in center of screen (assuming 1920x1080 resolution)
    player->SetPosition(glm::vec2(1920 / 2, 1080 / 2));

    for (int i = 0; i < 10; ++i) {
        Unit* npc = Unit::Pool->GetResource();
        npc->AssignNonDefValues();

        // Random position within 150 units of player
        float angle = static_cast<float>(rand()) / RAND_MAX * 2 * M_PI;
        float distance =  static_cast<float>(rand()) / RAND_MAX * 150.0f;

        npc->SetPosition(glm::vec2(
            player->GetPosition().x + cos(angle) * (5.0f + distance),
            player->GetPosition().y + sin(angle) * distance
        ));

        npc->SetScale(1.25f);
        npcWarriors.push_back(npc);
        npcTagged.push_back(false);
    }
}

void Level::HandleInput(SDL_Event event, float deltaTime)
{
    glm::vec2 offset(0.0f);
    float moveAmount = 100.0f * deltaTime;

    const Uint8* keys = SDL_GetKeyboardState(NULL);

    if ((event.type == SDL_QUIT) || 
        (input->KB()->KeyUp(event, SDLK_ESCAPE)))
    {
        m_quit = true;
    }
    if (keys[SDL_SCANCODE_A]) {
        offset.x -= moveAmount;
        player->IsMoving();
    }
    if (keys[SDL_SCANCODE_D]) {
        offset.x += moveAmount;
        player->IsMoving();
    }
    if (keys[SDL_SCANCODE_W]) {
        offset.y -= moveAmount;
        player->IsMoving();
    }
    if (keys[SDL_SCANCODE_S]) {
        offset.y += moveAmount;
        player->IsMoving();
    }
    if (offset.x != 0 && offset.y != 0) {
        offset = glm::normalize(offset) * moveAmount;
    }
    if (offset.x != 0 || offset.y != 0) {
        player->SetPosition(player->position + offset);
    }

}
void Level::Render(Renderer* renderer, Timing* timing) {
    renderer->SetDrawColor(Color(255, 255, 255, 255));
    renderer->ClearScreen();

    // Render player
    if (player) {
        Rect srcRect = player->Update(player->IsMoving() ? EN_AN_RUN : EN_AN_IDLE,
            timing->GetDeltaTime());

        unsigned int width = static_cast<unsigned int>(69 * player->GetScale());
        unsigned int height = static_cast<unsigned int>(44 * player->GetScale());
        unsigned int x = static_cast<unsigned int>(player->GetPosition().x - width / 2);
        unsigned int y = static_cast<unsigned int>(player->GetPosition().y - height / 2);

        Rect destRect(x, y, x + width, y + height);

        renderer->RenderTexture(player->GetWarrior(),
            srcRect,
            destRect, 255
        );
    }

    // Render NPC warriors
    for (size_t i = 0; i < npcWarriors.size(); ++i) {
        Unit* npc = npcWarriors[i];
        Rect srcRect = npc->Update(EN_AN_RUN, timing->GetDeltaTime());

        unsigned int width = static_cast<unsigned int>(69 * npc->GetScale());
        unsigned int height = static_cast<unsigned int>(44 * npc->GetScale());
        unsigned int x = static_cast<unsigned int>(npc->GetPosition().x - width / 2);
        unsigned int y = static_cast<unsigned int>(npc->GetPosition().y - height / 2);

        Rect destRect(x, y, x + width, y + height);

        // Use different alpha/color for tagged NPCs
        Uint8 alpha = npcTagged[i] ? 150 : 255;
        renderer->RenderTexture(npc->GetWarrior(),
            srcRect,
            destRect,
            alpha
        );

        // Draw a rectangle around tagged NPCs
        if (npcTagged[i]) {
            // Calculate rectangle position (centered around NPC)
            float rectX = npc->GetPosition().x - 20 / 2;
            float rectY = npc->GetPosition().y - 20 / 2;

            // Create rectangle slightly larger than NPC
            Rect tagRect(rectX, rectY,
                rectX + 20,
                rectY + 20);

            // Set color to red with some transparency
            renderer->SetDrawColor(Color(255, 0, 0, 180));
            renderer->RenderRectangle(tagRect);
        }

    }
}

void Level::Update(float deltaTime) {
    if (player) {
        player->UpdateMovement(deltaTime);
        TagNearbyNPCs();

        for (Unit* npc : npcWarriors) {
            glm::vec2 toPlayer = player->GetPosition() - npc->GetPosition();
            float distance = glm::length(toPlayer);
            glm::vec2 direction = toPlayer / distance;

            // Set facing direction
            npc->SetFacingRight(direction.x > 0);

            // Movement with minimum 5-unit distance
            if (distance < 5.0f) {
                // Push away strongly if too close
                npc->SetPosition(npc->GetPosition() - direction * 100.0f * deltaTime);
            }
            else if (distance < 140.0f) {
                // Normal retreat
                npc->SetPosition(npc->GetPosition() - direction * 60.0f * deltaTime);
            }
            else if (distance > 160.0f) {
                // Approach
                npc->SetPosition(npc->GetPosition() + direction * 60.0f * deltaTime);
            }

            npc->Update(EN_AN_RUN, deltaTime);
        }
    }
    physics->Update(deltaTime);
}

void Level::TagNearbyNPCs() {
    if (!player) return;

    glm::vec2 playerPos = player->GetPosition();

    for (size_t i = 0; i < npcWarriors.size(); ++i) {
        Unit* npc = npcWarriors[i];
        float distance = glm::distance(playerPos, npc->GetPosition());

        // Tag if within 30 units, untag if farther away
        npcTagged[i] = (distance < 30.0f);
    }
}

