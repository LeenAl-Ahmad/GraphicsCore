#include "Level.h"

Level::Level(int levelNumber) : font(nullptr), sheet(nullptr), startTime(0), isCompleted(false), levelNumber(levelNumber) {}

Level::~Level() {
    if (sheet) {
        delete sheet;
        sheet = nullptr;
    }
    if (rockSheet) {
        delete rockSheet;
        rockSheet = nullptr;
    }
    if (font) {
        font->Shutdown();
        delete font;
        font = nullptr;
    }
}

void Level::Initialize(Renderer* renderer) {
    font = new TTFont();
    font->Initialize(20);

    sheet = SpriteSheet::Pool->GetResource();
    sheet->Load("./Assets/Textures/Warrior.tga");
    sheet->SetSize(17, 6, 69, 44);
    sheet->AddAnimation(EN_AN_RUN, 6, 8, 0.00008);
    sheet->AddAnimation(EN_AN_DEATH, 26, 11, 0.00008);

    if (levelNumber == 1) {
        for (int i = 0; i < 10; ++i) {
            Warrior warrior;
            warrior.y = 10 + i * 100;
            warrior.x = 0;
            warrior.speed = 80 + (rand() % 21);
            warrior.animationSpeed = 4.8f + (rand() % (int)(warrior.speed - 80));
            warrior.currentFrame = sheet->GetCurrentClip(EN_AN_RUN);
            warrior.state = 0;
            warrior.deathAnimationTime = 0.0f;
            warrior.deathAnimationFrame = 0;
            warriors.push_back(warrior);
        }
    }
    else if (levelNumber == 2) {
        rockSheet = SpriteSheet::Pool->GetResource();
        rockSheet->Load("./Assets/Textures/Rock.tga");
        rockSheet->SetSize(1, 4, 20, 22);
        rockSheet->AddAnimation(EN_AN_ROCK_FALL, 0, 4, 0.0002f);

        for (int i = 0; i < 10; ++i) {
            Rock rock;
            rock.x = ((1920 - 2 * 50) / 9) * i;
            rock.y = 0;
            rock.speed = 80 + (rand() % 21);
            rock.animationSpeed = 4.8f + (rock.speed - 80) * (1.2f / 20);
            rock.currentFrame = 0;
            rocks.push_back(rock);
        }

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
        UpdateWarriors(deltaTime);

        int warriorsCompleted = 0;
        for (const auto& warrior : warriors) {
            if (warrior.x > 1920) {
                warriorsCompleted++;
            }
        }

        if (warriorsCompleted == warriors.size()) {
            isCompleted = true;
        }
    }
    else if (levelNumber == 2) {
        UpdateWarriors(deltaTime);
        UpdateRocks(deltaTime);

        // Check if all warriors are dead
        if (warriors.empty()) {
            isCompleted = true;
        }
    }

    if (!autoSaved && gameTime >= 5) {
        std::string filename = "Level" + std::to_string(levelNumber) + ".bin";
        std::ofstream outFile(filename, std::ios::binary);
        if (outFile.is_open()) {
            Serialize(outFile);
            outFile.close();
            autoSaved = true;

            std::ifstream inFile(filename, std::ios::binary);
            if (inFile.is_open()) {
                Deserialize(inFile);
                inFile.close();
            }
        }
    }

    if (isCompleted && levelNumber == 2) {
        SDL_Delay(2000); // Wait for 2 seconds before quitting
        SDL_Quit();
        exit(0);
    }
}

void Level::UpdateWarriors(float deltaTime) {
    for (auto it = warriors.begin(); it != warriors.end(); ) {
        if (it->state == 0) { // Alive
            it->x += it->speed * deltaTime;
            float animationTime = gameTime * it->animationSpeed;
            it->currentFrame = static_cast<int>(animationTime) % 8;

            if (it->x > 1920) {
                isCompleted = true;
            }

            ++it;
        }
        else if (it->state == 1) { // Dying
            it->deathAnimationTime += deltaTime;
            it->deathAnimationFrame = static_cast<int>(it->deathAnimationTime * it->animationSpeed);

            if (it->deathAnimationFrame >= 11) {
                it->state = 2;
            }

            ++it;
        }
        else if (it->state == 2) { // Dead
            it = warriors.erase(it);
        }
    }
}

void Level::UpdateRocks(float deltaTime) {
    for (auto it = rocks.begin(); it != rocks.end(); ) {
        bool rockRemoved = false;

        it->y += it->speed * deltaTime;
        float animationTime = gameTime * it->animationSpeed;
        it->currentFrame = static_cast<int>(animationTime) % 4;

        Rect rockRect(it->x, it->y, it->x + 20, it->y + 22);
        for (auto& warrior : warriors) {
            if (warrior.state == 0) {
                Rect warriorRect(warrior.x, warrior.y, warrior.x + 69, warrior.y + 44);

                if (rockRect.Overlaps(warriorRect)) {
                    it = rocks.erase(it);
                    rockRemoved = true;
                    warrior.state = 1;
                    warrior.deathAnimationTime = 0.0f;
                    warrior.deathAnimationFrame = 0;
                    break;
                }
            }
        }

        if (!rockRemoved) {
            ++it;
        }
    }
}

void Level::Render(Renderer* renderer, Timing* timing) {
    if (levelNumber == 1) {
        renderer->SetDrawColor(Color(128, 128, 128, 255));
    }
    else if (levelNumber == 2) {
        renderer->SetDrawColor(Color(0, 128, 0, 255));
    }

    renderer->ClearScreen();

    int i = 0;
    for (const auto& warrior : warriors) {
        float scale = 1.8f;
        Rect destRect(warrior.x, warrior.y, warrior.x + 69 * scale, warrior.y + (44 * scale));
        if (warrior.state == 0) {
            renderer->RenderTexture(sheet, sheet->Update(EN_AN_RUN, warrior.currentFrame), destRect);
        }
        else if (warrior.state == 1) {
            renderer->RenderTexture(sheet, sheet->Update(EN_AN_DEATH, warrior.deathAnimationFrame), destRect);
        }

        i++;
    }

    if (levelNumber == 2) {
        for (const auto& rock : rocks) {
            float scale = 1.0f;
            Rect destRect(rock.x, rock.y, rock.x + 20 * scale, rock.y + (22 * scale));
            renderer->RenderTexture(rockSheet, rockSheet->Update(EN_AN_ROCK_FALL, rock.currentFrame), destRect);
        }
    }

    std::string fps = "FPS: " + std::to_string(timing->GetFPS());
    font->Write(renderer->GetRenderer(), fps.c_str(), SDL_Color{ 0, 0, 255 }, SDL_Point{ 0, 0 });

    std::string timeText = "Game Time: " + std::to_string(gameTime) + "s";
    font->Write(renderer->GetRenderer(), timeText.c_str(), SDL_Color{ 0, 0, 255 }, SDL_Point{ 140, 0 });

    std::string saveStatus = autoSaved ? "Auto-saved at 5s" : "Not auto-saved yet";
    font->Write(renderer->GetRenderer(), saveStatus.c_str(), SDL_Color{ 0, 0, 255 }, SDL_Point{ 350, 0 });

    SDL_RenderPresent(renderer->GetRenderer());
}

bool Level::IsComplete() {
    return isCompleted;
}

void Level::Serialize(std::ostream& _stream) {
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

