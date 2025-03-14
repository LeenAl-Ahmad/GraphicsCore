#include "GameController.h"
#include "Renderer.h"
#include "SpriteSheet.h"
#include "TTFont.h"
#include "Timing.h"

GameController::GameController()
{
    m_sdlEvent = { }; 
}

GameController::~GameController()
{
}

void GameController::RunGame()
{
    AssetController::Instance().Initialize(10000000); // Allocate 10MB
    Renderer* r = &Renderer::Instance();
    Timing* t = &Timing::Instance();
    r->Initialize(800, 600);
    
    Point ws = r->GetWindowSize();

    TTFont* font = new TTFont();
    font->Initialize(20);

    // Initialize pools
    SpriteSheet::Pool = new ObjectPool<SpriteSheet>();
    SpriteAnim::Pool = new ObjectPool<SpriteAnim>();

    // Load and serialize SpriteSheet
    SpriteSheet* sheet = SpriteSheet::Pool->GetResource();
    sheet->Load("./Assets/Textures/Warrior.tga");
    sheet->SetSize(17, 6, 69, 44);
    sheet->AddAnimation(EN_AN_IDLE, 0, 6, 6.0f);
    sheet->AddAnimation(EN_AN_RUN, 6, 8, 6.0f);
    sheet->SetBlendMode(SDL_BLENDMODE_BLEND);
    sheet->SetBlandAlpha(128);
    /*ofstream writeStream("resource.bin", ios::out | ios::binary);
    sheet->Serialize(writeStream);
    writeStream.close();

    // Clear and reinitialize pools
    delete SpriteAnim::Pool;
    delete SpriteSheet::Pool;
    AssetController::Instance().Clear();
    AssetController::Instance().Initialize(10000000);
    SpriteSheet::Pool = new ObjectPool<SpriteSheet>();
    SpriteAnim::Pool = new ObjectPool<SpriteAnim>();

    // Deserialize SpriteSheet
    SpriteSheet* sheet2 = SpriteSheet::Pool->GetResource();
    ifstream readStream("resource.bin", ios::in | ios::binary);
    sheet2->Deserialize(readStream);
    readStream.close();*/

    // Main game loop
    while (m_sdlEvent.type != SDL_QUIT)
    {
        t->Tick();

        SDL_PollEvent(&m_sdlEvent);
        r->SetDrawColor(Color(255, 255, 255, 255));
        r->ClearScreen();
        r->RenderTexture(sheet, sheet->Update(EN_AN_IDLE, t->GetDeltaTime()), Rect(0, 0, 69 * 3, 44 * 3));
        r->RenderTexture(sheet, sheet->Update(EN_AN_RUN, t->GetDeltaTime()), Rect(0, 150, 69 * 3, 150 + 44 * 3));

        std::string s = "Frame number: " + std::to_string(sheet->GetCurrentClip(EN_AN_IDLE));
        font->Write(r->GetRenderer(),s.c_str(), SDL_Color{0, 255, 0}, SDL_Point{250, 50});

        s = "Frame number: " + std::to_string(sheet->GetCurrentClip(EN_AN_RUN));
        font->Write(r->GetRenderer(), s.c_str(), SDL_Color{ 0, 255, 0 }, SDL_Point{ 250, 200 });

        std::string fps = "Frames Per Second: " + std::to_string(t->GetFPS());
        font->Write(r->GetRenderer(), fps.c_str(), SDL_Color{ 0, 0, 255 }, SDL_Point{ 0, 0 });

        SDL_RenderPresent(r->GetRenderer());
        
        t->CapFPS();
    }

    // Clean up
    delete SpriteAnim::Pool;
    delete SpriteSheet::Pool;

    font->Shutdown();
    r->Shutdown();
}

