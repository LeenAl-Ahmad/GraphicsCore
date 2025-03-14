#include "GameController.h"
#include "Renderer.h"
#include "SpriteSheet.h"
#include "TTFont.h"

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
    sheet->AddAnimation(EN_AN_IDLE, 0, 6, 0.01f);
    sheet->AddAnimation(EN_AN_RUN, 6, 8, 0.005f);

    ofstream writeStream("resource.bin", ios::out | ios::binary);
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
    readStream.close();

    // Main game loop
    while (m_sdlEvent.type != SDL_QUIT)
    {
        SDL_PollEvent(&m_sdlEvent);
        r->SetDrawColor(Color(255, 255, 255, 255));
        r->ClearScreen();
        r->RenderTexture(sheet2, sheet2->Update(EN_AN_IDLE), Rect(0, 0, 69 * 3, 44 * 3));
        r->RenderTexture(sheet2, sheet2->Update(EN_AN_RUN), Rect(0, 150, 69 * 3, 150 + 44 * 3));

        std::string s = "Frame  number: " + std::to_string(sheet2->GetCurrentClip(EN_AN_IDLE));
        font->Write(r->GetRenderer(),s.c_str(), SDL_Color{0, 255, 0}, SDL_Point{250, 50});

        s = "Frame number: " + std::to_string(sheet2->GetCurrentClip(EN_AN_RUN));
        font->Write(r->GetRenderer(), s.c_str(), SDL_Color{ 0, 255, 0 }, SDL_Point{ 250, 200 });

        SDL_RenderPresent(r->GetRenderer());
    }

    // Clean up
    delete SpriteAnim::Pool;
    delete SpriteSheet::Pool;

    //font->Shutdown();
    r->Shutdown();
}

