#include "Level.h"

Level::Level(int levelNumber) : font(nullptr), sheet(nullptr), startTime(0), isCompleted(false), levelNumber(levelNumber) {}

Level::~Level() {
    // Do NOT delete sheet manually, it's managed by the object pool
    sheet = nullptr;

    if (font) {
        font->Shutdown();
        delete font;
    }
}

void Level::Initialize(Renderer* renderer) {
    // Initialize font
    font = new TTFont();
    font->Initialize(20);

    if (!SpriteSheet::Pool) {
        std::cerr << "Error: SpriteSheet::Pool is null!" << std::endl;
        return;
    }
    // Initialize sprite sheet
    sheet = SpriteSheet::Pool->GetResource();
    sheet->Load("./Assets/Textures/Warrior.tga");
    sheet->SetSize(17, 6, 69, 44);
    sheet->AddAnimation(EN_AN_RUN, 6, run, 0.0002f); // "Run" animation (starts at frame 6, 8 frames)

    // Initialize warriors
    for (int i = 0; i < 10; ++i) {
        Warrior warrior;
        warrior.y = 10 + i * 100; // Y positions: 10, 110, 210, etc.
        warrior.x = 0; // Start at the left of the screen
        warrior.speed = 80 + (rand() % 21); // Random speed between 80 and 100
        warrior.animationSpeed = 4.8f + (warrior.speed - 80) * (1.2f / 20); // Animation speed scales with running speed
        warrior.currentFrame = sheet->GetCurrentClip(EN_AN_RUN); // Start at frame 0
        warriors.push_back(warrior);
    }

    startTime = SDL_GetTicks();
    autoSaved = false;
    gameTime = 0;
}

void Level::Update(float deltaTime) {
    // Update game time
    gameTime = (SDL_GetTicks() - startTime) / 1000;

    // Update warrior positions and frames
    for (auto& warrior : warriors) {
        warrior.x += warrior.speed * deltaTime;

        // Calculate animation frame based on elapsed time and animation speed
        float animationTime = gameTime * warrior.animationSpeed;
        warrior.currentFrame = static_cast<int>(animationTime) % run;

        // Check if the warrior has gone off-screen
        if (warrior.x > 800) { // Assuming screen width is 800
            isCompleted = true; // Level is complete when the first warrior goes off-screen
            break;
        }
    }

    // Auto-save after 5 seconds
    if (!autoSaved && gameTime >= 5) {
        Save("Level1.bin");
        autoSaved = true;
    }
}

void Level::Render(Renderer* renderer, Timing* timing) {
    if (levelNumber == 1) {
        renderer->SetDrawColor(Color(128, 128, 128, 255)); // Grey background
    }
    else if (levelNumber == 2) {
        renderer->SetDrawColor(Color(0, 128, 0, 255)); // Green background
    }

    renderer->ClearScreen();

    // Render warriors
    int i = 0;
    for (const auto& warrior : warriors) {
        // Destination rectangle (scaled by 1.8x)
        float scale = 1.8f;
        float additionalHeight = (i + 1);
        Rect destRect(warrior.x, warrior.y + additionalHeight, warrior.x + 69 * scale, warrior.y + additionalHeight + (44 * scale));

        // Render the warrior with the current frame
        renderer->RenderTexture(sheet, sheet->Update(EN_AN_RUN, warrior.currentFrame), destRect);
        i++;
    }

    // Render UI (FPS, Game time, Save status, etc.)
    std::string fps = "FPS: " + std::to_string(timing->GetFPS());
    font->Write(renderer->GetRenderer(), fps.c_str(), SDL_Color{ 0, 0, 255 }, SDL_Point{ 0, 0 });

    std::string timeText = "Game Time: " + std::to_string(gameTime) + "s";
    font->Write(renderer->GetRenderer(), timeText.c_str(), SDL_Color{ 0, 0, 255 }, SDL_Point{ 140, 0 });

    std::string saveStatus = autoSaved ? "Auto-saved at 5s" : "Not auto-saved yet";
    font->Write(renderer->GetRenderer(), saveStatus.c_str(), SDL_Color{ 0, 0, 255 }, SDL_Point{ 350, 0 });

    // Present the rendered frame
    SDL_RenderPresent(renderer->GetRenderer());
}

bool Level::IsComplete() {
    return isCompleted; // Implement level completion logic
}

void Level::Save(const std::string& filename) {
    std::ofstream outFile(filename, std::ios::binary);
    if (outFile.is_open()) {
        // Save warrior data
        for (const auto& warrior : warriors) {
            outFile.write(reinterpret_cast<const char*>(&warrior), sizeof(Warrior));
        }

        // Save game time
        outFile.write(reinterpret_cast<const char*>(&gameTime), sizeof(gameTime));

        outFile.close();
        std::cout << "Level saved to " << filename << std::endl;
    }
    else {
        std::cerr << "Failed to save level to " << filename << std::endl;
    }
}


void Level::Load(const std::string& filename) {
    std::ifstream inFile(filename, std::ios::binary);
    if (inFile.is_open()) {
        // Load warrior data
        warriors.clear();
        for (int i = 0; i < 10; ++i) {
            Warrior warrior;
            inFile.read(reinterpret_cast<char*>(&warrior), sizeof(Warrior));
            warriors.push_back(warrior);
        }

        // Load game time
        inFile.read(reinterpret_cast<char*>(&gameTime), sizeof(gameTime));

        inFile.close();
        std::cout << "Level loaded from " << filename << std::endl;
    }
    else {
        std::cerr << "Failed to load level from " << filename << std::endl;
    }
}

