#include "Level.h"

Level::Level() : font(nullptr), sheet(nullptr) {}

Level::~Level() {
    if (sheet) {
        delete sheet;
    }
    if (font) {
        font->Shutdown();
        delete font;
    }
}

void Level::Initialize(Renderer* renderer) {
    // Initialize font
    font = new TTFont();
    font->Initialize(20);

    // Initialize sprite sheet
    sheet = SpriteSheet::Pool->GetResource();
    sheet->Load("./Assets/Textures/Warrior.tga");
    sheet->SetSize(17, 6, 69, 44);
    sheet->AddAnimation(EN_AN_IDLE, 0, 6, 6.0f);
    sheet->AddAnimation(EN_AN_RUN, 6, 8, 6.0f);
}

void Level::Update(float deltaTime) {
    // Update animations
    sheet->Update(EN_AN_IDLE, deltaTime);
    sheet->Update(EN_AN_RUN, deltaTime);
}

void Level::Render(Renderer* renderer, Timing* timing) {
    // Clear screen
    renderer->SetDrawColor(Color(255, 255, 255, 255));
    renderer->ClearScreen();

    // Render animations
    renderer->RenderTexture(sheet, sheet->Update(EN_AN_IDLE, timing->GetDeltaTime()), Rect(0, 0, 69 * 3, 44 * 3));
    renderer->RenderTexture(sheet, sheet->Update(EN_AN_RUN, timing->GetDeltaTime()), Rect(0, 150, 69 * 3, 150 44 * 3));
    // Render frame numbers
    std::string idleFrame = "Frame number: " + std::to_string(sheet->GetCurrentClip(EN_AN_IDLE));
    font->Write(renderer->GetRenderer(), idleFrame.c_str(), SDL_Color{ 0, 255, 0 }, SDL_Point{ 250, 50 });

    std::string runFrame = "Frame number: " + std::to_string(sheet->GetCurrentClip(EN_AN_RUN));
    font->Write(renderer->GetRenderer(), runFrame.c_str(), SDL_Color{ 0, 255, 0 }, SDL_Point{ 250, 200 });

    // Render FPS
    std::string fps = "Frames per Second: " + std::to_string(timing->GetFPS());
    font->Write(renderer->GetRenderer(), fps.c_str(), SDL_Color{ 0, 0, 255 }, SDL_Point{ 0, 0 });

    // Present the rendered frame
    SDL_RenderPresent(renderer->GetRenderer());
}

bool Level::IsComplete() {
    return false; // Implement level completion logic
}