#include "Level.h"

Level::Level()
{
    physics = nullptr;
    player = nullptr;
    input = nullptr;
}

Level::~Level() {
    for (Unit* npc : npcWarriors) {
        if (npc) Unit::Pool->ReturnResource(npc);
    }
    npcWarriors.clear();
    npcTagged.clear();
    npcDeathTimers.clear();

    // Clean up player
    if (player) {
        Unit::Pool->ReturnResource(player);
        player = nullptr;
    }

    // Clean up pools if needed
    if (Unit::Pool) {
        delete Unit::Pool;
        Unit::Pool = nullptr;
    }
    if (SpriteSheet::Pool) {
        delete SpriteSheet::Pool;
        SpriteSheet::Pool = nullptr;
    }
    if (SpriteAnim::Pool) {
        delete SpriteAnim::Pool;
        SpriteAnim::Pool = nullptr;
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
    player->SetBlueTint();

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
        npcDeathTimers.push_back(0.0f);
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
    if (input->KB()->KeyUp(event, SDLK_ESCAPE)) {
        m_quit = true;
    }
    else if (input->KB()->KeyUp(event, SDLK_s)) {  // Save
        std::ofstream file("level.bin", std::ios::binary);
        if (file.is_open()) {
            Serialize(file);
            file.close();
            std::cout << "Game saved to level.bin" << std::endl;
        }
        else {
            std::cerr << "Failed to create level.bin" << std::endl;
        }
    }
    else if (input->KB()->KeyUp(event, SDLK_l)) {  // Load
        std::ifstream file("level.bin", std::ios::binary);
        if (file.is_open()) {
            // Clear current state
            for (Unit* npc : npcWarriors) {
                Unit::Pool->ReturnResource(npc);
            }
            npcWarriors.clear();
            npcTagged.clear();
            npcDeathTimers.clear();

            Deserialize(file);
            file.close();
            std::cout << "Game loaded from level.bin" << std::endl;
        }
        else {
            std::cerr << "Failed to load level.bin" << std::endl;
        }
    }
    static float npcSpeed = 60.0f; // Default/base speed (you can adjust this)

    if (input->KB()->KeyUp(event, SDLK_i)) { // Increase speed
        npcSpeed = std::min(npcSpeed + 10.0f, 60.0f); // Cap at 60
    }
    if (input->KB()->KeyUp(event, SDLK_d)) { // Decrease speed
        npcSpeed = std::max(npcSpeed - 10.0f, 0.0f); // Minimum 0
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

        // Get the tint color
        glm::vec4 tint = player->GetTint();

        // Convert to SDL color format (0-255)
        SDL_Color sdlTint = {
            static_cast<Uint8>(tint.r * 255),
            static_cast<Uint8>(tint.g * 255),
            static_cast<Uint8>(tint.b * 255),
            static_cast<Uint8>(tint.a * 255)
        };

        // OR if it only takes alpha:
        renderer->RenderTexture(player->GetWarrior(),
            srcRect,
            destRect,
            static_cast<Uint8>(tint.a * 255));
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

// In Level.cpp - Update()
void Level::Update(float deltaTime) {
    if (player) {
        player->UpdateMovement(deltaTime);
        TagNearbyNPCs();

        // Update NPCs
        for (size_t i = 0; i < npcWarriors.size(); ) {
            Unit* npc = npcWarriors[i];

            if (npcTagged[i]) {
                // 1. Stop movement
                // (No movement code for tagged NPCs)

                // 2. Update death animation timer
                npcDeathTimers[i] += deltaTime;

                // 3. Remove when animation complete
                if (npcDeathTimers[i] >= DEATH_ANIMATION_DURATION) {
                    Unit::Pool->ReturnResource(npc);
                    npcWarriors.erase(npcWarriors.begin() + i);
                    npcTagged.erase(npcTagged.begin() + i);
                    npcDeathTimers.erase(npcDeathTimers.begin() + i);
                    continue; // Skip increment since we removed an element
                }
            }
            else {
                // Existing movement code for untagged NPCs
                glm::vec2 toPlayer = player->GetPosition() - npc->GetPosition();
                float distance = glm::length(toPlayer);
                glm::vec2 direction = toPlayer / distance;

                npc->SetFacingRight(direction.x > 0);

                if (distance < 5.0f) {
                    npc->SetPosition(npc->GetPosition() - direction * npcSpeed * 1.67f * deltaTime); // 100/60 ≈ 1.67
                }
                else if (distance < 140.0f) {
                    npc->SetPosition(npc->GetPosition() - direction * npcSpeed * deltaTime);
                }
                else if (distance > 160.0f) {
                    npc->SetPosition(npc->GetPosition() + direction * npcSpeed * deltaTime);
                }
            }

            i++; // Only increment if we didn't remove an element
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

void Level::Serialize(std::ostream& _stream) {
    // Serialize basic level data
    _stream.write(reinterpret_cast<const char*>(&npcSpeed), sizeof(npcSpeed));
    _stream.write(reinterpret_cast<const char*>(&m_quit), sizeof(m_quit));

    // Serialize player
    bool hasPlayer = (player != nullptr);
    _stream.write(reinterpret_cast<const char*>(&hasPlayer), sizeof(hasPlayer));
    if (hasPlayer) {
        player->Serialize(_stream);
    }

    // Serialize NPCs
    int npcCount = npcWarriors.size();
    _stream.write(reinterpret_cast<const char*>(&npcCount), sizeof(npcCount));

    // Corrected serialization code
    for (size_t i = 0; i < npcWarriors.size(); i++) {
        npcWarriors[i]->Serialize(_stream);

        // Get references to the elements first
        bool tagged = npcTagged[i];
        float timer = npcDeathTimers[i];

        // Then take their addresses
        _stream.write(reinterpret_cast<const char*>(&tagged), sizeof(tagged));
        _stream.write(reinterpret_cast<const char*>(&timer), sizeof(timer));
    }

    
}

void Level::Deserialize(std::istream& _stream) {
    // Clear current state
    for (Unit* npc : npcWarriors) {
        Unit::Pool->ReturnResource(npc);
    }
    npcWarriors.clear();
    npcTagged.clear();
    npcDeathTimers.clear();

    // Deserialize basic level data
    _stream.read(reinterpret_cast<char*>(&npcSpeed), sizeof(npcSpeed));
    _stream.read(reinterpret_cast<char*>(&m_quit), sizeof(m_quit));

    // Deserialize player
    bool hasPlayer;
    _stream.read(reinterpret_cast<char*>(&hasPlayer), sizeof(hasPlayer));
    if (hasPlayer) {
        if (!player) {
            player = Unit::Pool->GetResource();
        }
        player->Deserialize(_stream);
    }

    // Deserialize NPCs
    int npcCount;
    _stream.read(reinterpret_cast<char*>(&npcCount), sizeof(npcCount));

    npcWarriors.reserve(npcCount);
    npcTagged.reserve(npcCount);
    npcDeathTimers.reserve(npcCount);

    for (int i = 0; i < npcCount; i++) {
        Unit* npc = Unit::Pool->GetResource();
        npc->Deserialize(_stream);
        npcWarriors.push_back(npc);

        bool tagged;
        float deathTimer;
        _stream.read(reinterpret_cast<char*>(&tagged), sizeof(tagged));
        _stream.read(reinterpret_cast<char*>(&deathTimer), sizeof(deathTimer));
        npcTagged.push_back(tagged);
        npcDeathTimers.push_back(deathTimer);
    }
}