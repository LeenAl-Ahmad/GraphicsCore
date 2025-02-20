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

    // Initialize warrior sprite sheet (for Level 1)
    sheet = SpriteSheet::Pool->GetResource();
    sheet->Load("./Assets/Textures/Warrior.tga");
    sheet->SetSize(17, 6, 69, 44);
    sheet->AddAnimation(EN_AN_RUN, 6, 8, 0.0002f); // "Run" animation
    sheet->AddAnimation(EN_AN_DEATH, 27, 11, 0.0002f); // "Death" animation

    if (levelNumber == 1) {
        // Initialize warriors for Level 1
        for (int i = 0; i < 10; ++i) {
            Warrior warrior;
            warrior.y = 10 + i * 100; // Y positions: 10, 110, 210, etc.
            warrior.x = 0; // Start at the left of the screen
            warrior.speed = 80 + (rand() % 21); // Random speed between 80 and 100
            warrior.animationSpeed = 4.8f + (warrior.speed - 80) * (1.2f / 20); // Animation speed scales with running speed
            warrior.currentFrame = sheet->GetCurrentClip(EN_AN_RUN); // Start at frame 0
            warrior.state = 0; // Alive
            warrior.deathAnimationTime = 0.0f; // No death animation yet
            warrior.deathAnimationFrame = 0; // Start at frame 0 of death animation
            warriors.push_back(warrior);
        }
    }
    else if (levelNumber == 2) {
        // Initialize rock sprite sheet (for Level 2)
        rockSheet = SpriteSheet::Pool->GetResource();
        rockSheet->Load("./Assets/Textures/Rock.tga");
        rockSheet->SetSize(1, 4, 20, 22);
        rockSheet->AddAnimation(EN_AN_ROCK_FALL, 0, 4, 0.0002f);

        // Initialize rocks for Level 2
        for (int i = 0; i < 10; ++i) {
            Rock rock;
            rock.x = 50 + i * 100; // X positions: 50, 150, 250, etc.
            rock.y = 0; // Start at the top of the screen
            rock.speed = 80 + (rand() % 21); // Random speed between 80 and 100
            rock.animationSpeed = 4.8f + (rock.speed - 80) * (1.2f / 20); // Animation speed scales with falling speed
            rock.currentFrame = 0; // Start at frame 0
            rocks.push_back(rock);
        }

        // Initialize warriors for Level 2
        for (int i = 0; i < 10; ++i) {
            Warrior warrior;
            warrior.y = 10 + i * 100; // Y positions: 10, 110, 210, etc.
            warrior.x = 0; // Start at the left of the screen
            warrior.speed = 80 + (rand() % 21); // Random speed between 80 and 100
            warrior.animationSpeed = 4.8f + (warrior.speed - 80) * (1.2f / 20); // Animation speed scales with running speed
            warrior.currentFrame = sheet->GetCurrentClip(EN_AN_RUN); // Start at frame 0
            warrior.state = 0; // Alive
            warrior.deathAnimationTime = 0.0f; // No death animation yet
            warrior.deathAnimationFrame = 0; // Start at frame 0 of death animation
            warriors.push_back(warrior);
        }
    }

    startTime = SDL_GetTicks();
    autoSaved = false;
    gameTime = 0;
}

void Level::Update(float deltaTime) {
    // Update game time
    gameTime = (SDL_GetTicks() - startTime) / 1000;

    if (levelNumber == 1) {
        // Update warrior positions and frames for Level 1
        for (auto& warrior : warriors) {
            warrior.x += warrior.speed * deltaTime;

            // Calculate animation frame based on elapsed time and animation speed
            float animationTime = gameTime * warrior.animationSpeed;
            warrior.currentFrame = static_cast<int>(animationTime) % 8;

            // Check if the warrior has gone off-screen
            if (warrior.x > 800) { // Assuming screen width is 800
                isCompleted = true; // Level is complete when the first warrior goes off-screen
                break;
            }
        }
    }
    else if (levelNumber == 2) {
        // Update warrior positions and frames for Level 2
        for (auto it = warriors.begin(); it != warriors.end(); ) {
            if (it->state == 0) { // Warrior is alive
                it->x += it->speed * deltaTime;

                // Calculate animation frame based on elapsed time and animation speed
                float animationTime = gameTime * it->animationSpeed;
                it->currentFrame = static_cast<int>(animationTime) % 8;

                // Check if the warrior has gone off-screen
                if (it->x > 800) { // Assuming screen width is 800
                    isCompleted = true; // Level is complete when the first warrior goes off-screen
                    break;
                }

                ++it;
            }
            else if (it->state == 1) { // Warrior is dying
                // Update death animation time
                it->deathAnimationTime += deltaTime;

                // Calculate death animation frame
                it->deathAnimationFrame = static_cast<int>(it->deathAnimationTime * it->animationSpeed);

                // Check if death animation is complete (11 frames)
                if (it->deathAnimationFrame >= 11) {
                    it->state = 2; // Mark warrior as dead
                }

                ++it;
            }
            else if (it->state == 2) { // Warrior is dead
                // Remove the warrior from the level
                it = warriors.erase(it);
            }
        }

        // Update rock positions and frames for Level 2
        for (auto it = rocks.begin(); it != rocks.end(); ) {
            bool rockRemoved = false;

            // Update rock position
            it->y += it->speed * deltaTime;

            // Calculate animation frame based on elapsed time and animation speed
            float animationTime = gameTime * it->animationSpeed;
            it->currentFrame = static_cast<int>(animationTime) % 4;

            // Define bounding box for the rock
            Rect rockRect(it->x, it->y, it->x + 20, it->y + 22); // Rock size: 20x22

            // Check for collision with warriors
            for (auto& warrior : warriors) {
                if (warrior.state == 0) { // Only check collisions with alive warriors
                    // Define bounding box for the warrior
                    Rect warriorRect(warrior.x, warrior.y, warrior.x + 69, warrior.y + 44); // Warrior size: 69x44

                    // Check if bounding boxes overlap
                    if (rockRect.Overlaps(warriorRect)) {
                        // Remove the rock
                        it = rocks.erase(it);
                        rockRemoved = true;

                        // Start death animation for the warrior
                        warrior.state = 1; // Dying
                        warrior.deathAnimationTime = 0.0f; // Reset death animation time
                        warrior.deathAnimationFrame = 0; // Reset death animation frame
                        break; // Exit the inner loop
                    }
                }
            }

            // If the rock was not removed, move to the next rock
            if (!rockRemoved) {
                ++it;
            }

            // Check if the rock has gone off-screen
            if (it != rocks.end() && it->y > 600) { // Assuming screen height is 600
                isCompleted = true; // Level is complete when the first rock goes off-screen
                break;
            }
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

    // Render warriors for both Level 1 and Level 2
    int i = 0;
    for (const auto& warrior : warriors) {
        // Destination rectangle (scaled by 1.8x)
        float scale = 1.8f;
        float additionalHeight = (i + 1);
        Rect destRect(warrior.x, warrior.y + additionalHeight, warrior.x + 69 * scale, warrior.y + additionalHeight + (44 * scale));

        // Render the warrior with the current frame
        if (warrior.state == 0) { // Alive
            renderer->RenderTexture(sheet, sheet->Update(EN_AN_RUN, warrior.currentFrame), destRect);
        }
        else if (warrior.state == 1) { // Dying
            renderer->RenderTexture(sheet, sheet->Update(EN_AN_DEATH, warrior.deathAnimationFrame), destRect);
        }

        i++;
    }

    if (levelNumber == 2) {
        // Render rocks for Level 2
        for (const auto& rock : rocks) {
            // Destination rectangle (scaled by 1.0x)
            float scale = 1.0f;
            Rect destRect(rock.x, rock.y, rock.x + 20 * scale, rock.y + (22 * scale));

            // Render the rock with the current frame
            renderer->RenderTexture(rockSheet, rockSheet->Update(EN_AN_ROCK_FALL, rock.currentFrame), destRect);
        }
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
        // Save warrior data (for both Level 1 and Level 2)
        for (const auto& warrior : warriors) {
            outFile.write(reinterpret_cast<const char*>(&warrior), sizeof(Warrior));
        }

        if (levelNumber == 2) {
            // Save rock data (for Level 2)
            for (const auto& rock : rocks) {
                outFile.write(reinterpret_cast<const char*>(&rock), sizeof(Rock));
            }
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
        // Load warrior data (for both Level 1 and Level 2)
        warriors.clear();
        for (int i = 0; i < 10; ++i) {
            Warrior warrior;
            inFile.read(reinterpret_cast<char*>(&warrior), sizeof(Warrior));
            warriors.push_back(warrior);
        }

        if (levelNumber == 2) {
            // Load rock data (for Level 2)
            rocks.clear();
            for (int i = 0; i < 10; ++i) {
                Rock rock;
                inFile.read(reinterpret_cast<char*>(&rock), sizeof(Rock));
                rocks.push_back(rock);
            }
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

