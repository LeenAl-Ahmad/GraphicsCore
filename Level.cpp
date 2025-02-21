#include "Level.h"

Level::Level(int levelNumber) : font(nullptr), sheet(nullptr), startTime(0), isCompleted(false), levelNumber(levelNumber) {}

Level::~Level() {
    sheet = nullptr;

    if (font) {
        font->Shutdown();
        delete font;
    }
}

void Level::Initialize(Renderer* renderer) 
{
    font = new TTFont();
    font->Initialize(20);

    // Initialize warrior (for Level 1)
    sheet = SpriteSheet::Pool->GetResource();
    sheet->Load("./Assets/Textures/Warrior.tga");
    sheet->SetSize(17, 6, 69, 44);
    sheet->AddAnimation(EN_AN_RUN, 6, 8, 0.0003);
    sheet->AddAnimation(EN_AN_DEATH, 27, 11, 0.0004);

    if (levelNumber == 1) {
        for (int i = 0; i < 10; ++i) {
            Warrior warrior;
            warrior.y = 10 + i * 100; // Y positions: 10, 110, 210, etc.
            warrior.x = 0; 
            warrior.speed = 80 + (rand() % 21); // Random speed between 80 and 100
            warrior.animationSpeed = 4.8f + (rand() % (int)(warrior.speed - 80)); // Animation speed scales with running speed
            warrior.currentFrame = sheet->GetCurrentClip(EN_AN_RUN); // Start at frame 0
            warrior.state = 0; // Alive
            warrior.deathAnimationTime = 0.0f; // No death animation yet
            warrior.deathAnimationFrame = 0; // Start at frame 0 of death animation
            warriors.push_back(warrior);
        }
    }
    else if (levelNumber == 2) {
        // Initialize (for Level 2)
        rockSheet = SpriteSheet::Pool->GetResource();
        rockSheet->Load("./Assets/Textures/Rock.tga");
        rockSheet->SetSize(1, 4, 20, 22);
        rockSheet->AddAnimation(EN_AN_ROCK_FALL, 0, 4, 0.0002f);

        for (int i = 0; i < 10; ++i) {
            Rock rock;
            rock.x =((1920 - 2 * 50) / 9)* i; // X positions: 50, 150, 250, etc...
            rock.y = 0;
            rock.speed = 80 + (rand() % 21); // Random speed between 80 and 100
            rock.animationSpeed = 4.8f + (rock.speed - 80) * (1.2f / 20);
            rock.currentFrame = 0; 
            rocks.push_back(rock);
        }

        // Initialize warriors 
        for (int i = 0; i < 10; ++i) {
            Warrior warrior;
            warrior.y = 10 + i * 100; 
            warrior.x = 0;
            warrior.speed = 80 + (rand() % 21); 
            warrior.animationSpeed = 4.8f + (warrior.speed - 80) * (1.2f / 20); 
            warrior.currentFrame = sheet->GetCurrentClip(EN_AN_RUN); 
            warrior.state = 0;
            warrior.deathAnimationTime = 0.0f; 
            warrior.deathAnimationFrame = 0;
            warriors.push_back(warrior);
        }
    }

    startTime = SDL_GetTicks();
    autoSaved = false;
    gameTime = 0;
}

void Level::Update(float deltaTime) {
    gameTime = (SDL_GetTicks() - startTime) / 1000;

    if (levelNumber == 1) {
        
        for (auto& warrior : warriors) {
            warrior.x += warrior.speed * deltaTime;

            // Calculate animation frame based on elapsed time and animation speed
            float animationTime = gameTime * warrior.animationSpeed;
            warrior.currentFrame = static_cast<int>(animationTime) % 8;

            if (warrior.x > 1920) {
                isCompleted = true; 
                break;
            }
        }
    }
    
    else if (levelNumber == 2) {
        
        for (auto it = warriors.begin(); it != warriors.end(); ) {
            if (it->state == 0) { //alive
                it->x += it->speed * deltaTime;
                float animationTime = gameTime * it->animationSpeed;
                it->currentFrame = static_cast<int>(animationTime) % 8;
                if (it->x > 1920) {
                    isCompleted = true; 
                    break;
                }

                ++it;
            }
            else if (it->state == 1) { // dying

                it->deathAnimationTime += deltaTime;
                it->deathAnimationFrame = static_cast<int>(it->deathAnimationTime * it->animationSpeed);
                //(11 frames)
                if (it->deathAnimationFrame >= 11) {
                    it->state = 2; 
                }
                ++it;
            }
            else if (it->state == 2) { // dead
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
            if (it != rocks.end() && it->y > 1080) { // Assuming screen height is 600
                isCompleted = true; // Level is complete when the first rock goes off-screen
                break;
           
            }
        }
    }

    if (!autoSaved && levelNumber == 1 && gameTime >= 5) {
        std::ofstream outFile("Level1.bin", std::ios::binary);
        if (outFile.is_open()) {
            Serialize(outFile);
            outFile.close();
            autoSaved = true;

            std::ifstream inFile("Level1.bin", std::ios::binary);
            if (inFile.is_open()) {
                Deserialize(inFile);
                inFile.close();
            }
        }
    }
    if (!autoSaved && levelNumber == 2 && gameTime >= 5) {
        std::ofstream outFile("Level2.bin", std::ios::binary);
        if (outFile.is_open()) {
            Serialize(outFile);
            outFile.close();
            autoSaved = true;

            std::ifstream inFile("Level2.bin", std::ios::binary);
            if (inFile.is_open()) {
                Deserialize(inFile);
                inFile.close();
            }
        }
    }
    if (isCompleted && levelNumber == 2) {
        SDL_Quit();
        exit(0);
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

    int i = 0;
    for (const auto& warrior : warriors) {
        // scaled by 1.8x
        float scale = 1.8f;
        Rect destRect(warrior.x, warrior.y , warrior.x + 69 * scale, warrior.y  + (44 * scale));
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
            float scale = 1.0f;
            Rect destRect(rock.x, rock.y, rock.x + 20 * scale, rock.y + (22 * scale));
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

void Level::Serialize(std::ostream& _stream){
    _stream.write(reinterpret_cast<const char*>(&levelNumber), sizeof(levelNumber));

    size_t warriorCount = warriors.size();
    _stream.write(reinterpret_cast<const char*>(&warriorCount), sizeof(warriorCount));
    for (const auto& warrior : warriors) {
        _stream.write(reinterpret_cast<const char*>(&warrior), sizeof(Warrior));
    }
    size_t rockCount = rocks.size();
    _stream.write(reinterpret_cast<const char*>(&rockCount), sizeof(rockCount));
    for (const auto& rock : rocks) {
        _stream.write(reinterpret_cast<const char*>(&rock), sizeof(Rock));
    }

    _stream.write(reinterpret_cast<const char*>(&gameTime), sizeof(gameTime));
    _stream.write(reinterpret_cast<const char*>(&isCompleted), sizeof(isCompleted));
}

void Level::Deserialize(std::istream& _stream) {
    _stream.read(reinterpret_cast<char*>(&levelNumber), sizeof(levelNumber));

    size_t warriorCount;
    _stream.read(reinterpret_cast<char*>(&warriorCount), sizeof(warriorCount));
    warriors.clear();
    for (size_t i = 0; i < warriorCount; ++i) {
        Warrior warrior;
        _stream.read(reinterpret_cast<char*>(&warrior), sizeof(Warrior));
        warriors.push_back(warrior);
    }

    size_t rockCount;
    _stream.read(reinterpret_cast<char*>(&rockCount), sizeof(rockCount));
    rocks.clear();
    for (size_t i = 0; i < rockCount; ++i) {
        Rock rock;
        _stream.read(reinterpret_cast<char*>(&rock), sizeof(Rock));
        rocks.push_back(rock);
    }

    _stream.read(reinterpret_cast<char*>(&gameTime), sizeof(gameTime));
    _stream.read(reinterpret_cast<char*>(&isCompleted), sizeof(isCompleted));
}