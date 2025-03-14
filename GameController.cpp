#include "GameController.h"
#include "Renderer.h"
#include "TTFont.h"
#include "InputController.h"
#include "Keyboard.h"

GameController::GameController()
{
    m_sdlEvent = { }; 
    m_renderer = nullptr;
    m_fArial20 = nullptr;
    m_quit = false;
    m_input = nullptr;
    m_text = "";
}

GameController::~GameController()
{
    ShutDown();
}

void GameController::Initialize()
{
    m_renderer = &Renderer::Instance();
    m_renderer->Initialize();
    m_input = &InputController::Instance();
    m_fArial20 = new TTFont();
    m_fArial20->Initialize(20);
}

void GameController::HandleInput(SDL_Event _event)
{
    string temp;
    if((m_sdlEvent.type == SDL_QUIT)||(m_input->KB()->KeyUp(m_sdlEvent, SDLK_ESCAPE)))
    { m_quit = TRUE; }
    else if ((temp = m_input->KB()->TextInput(_event)) != "")
    { m_text += temp; }
    else if (m_input->KB()->KeyUp(m_sdlEvent, SDLK_RETURN))
    { m_text = ""; }
}

void GameController::ShutDown()
{
    delete m_fArial20;
}

void GameController::RunGame()
{
    Initialize();

    while (!m_quit)
    {
        m_renderer->SetDrawColor(Color(255, 255, 255, 255));
        m_renderer->ClearScreen();

        while (SDL_PollEvent(&m_sdlEvent) != 0)
        {
            HandleInput(m_sdlEvent);
        }

        m_fArial20->Write(m_renderer->GetRenderer(), m_text.c_str(), SDL_Color{ 0,255,0 }, SDL_Point{ 250, 200 });
        SDL_RenderPresent(m_renderer->GetRenderer());
    }
    /*AssetController::Instance().Initialize(10000000); // Allocate 10MB
    Renderer* r = &Renderer::Instance();
    Timing* t = &Timing::Instance();
    r->Initialize();
    r->EnumerateDisplayMode();
    r->ChangeDisplayMode(&r->GetResolutions()[8]);

    TTFont* font = new TTFont();
    font->Initialize(20);

    // Initialize pools
    SpriteSheet::Pool = new ObjectPool<SpriteSheet>();
    SpriteAnim::Pool = new ObjectPool<SpriteAnim>();
    SpriteSheet* sheet = SpriteSheet::Pool->GetResource();
    sheet->Load("./Assets/Textures/Warrior.tga");
    sheet->SetSize(17, 6, 69, 44);
    sheet->AddAnimation(EN_AN_IDLE, 0, 6, 6.0f);
    sheet->AddAnimation(EN_AN_RUN, 6, 8, 6.0f);

    RenderTarget* rt = new RenderTarget;
    rt->Create(NATIVE_XRES, NATIVE_YRES);

    // Main game loop
    while (m_sdlEvent.type != SDL_QUIT)
    {
        t->Tick();
        rt->Start();
        r->SetDrawColor(Color(255, 255, 255, 255));
        r->ClearScreen();
        r->RenderTexture(sheet, sheet->Update(EN_AN_RUN, t->GetDeltaTime()), Rect(0, 150, 69 * 3, 150 + 44 * 3));

        std::string fps = "Frames Per Second: " + std::to_string(t->GetFPS());
        font->Write(r->GetRenderer(), fps.c_str(), SDL_Color{ 0, 0, 255 }, SDL_Point{ 0, 0 });
        std::string s = "Frame number: " + std::to_string(sheet->GetCurrentClip(EN_AN_RUN));
        font->Write(r->GetRenderer(), s.c_str(), SDL_Color{ 0, 255, 0 }, SDL_Point{ 250, 200 });

        rt->stop();
        r->SetDrawColor(Color(0, 0, 0, 255));
        r->ClearScreen();
        rt->Render(t->GetDeltaTime());
        SDL_RenderPresent(r->GetRenderer()); 
        t->CapFPS();
    }

    // Clean up
    delete rt;
    delete SpriteAnim::Pool;
    delete SpriteSheet::Pool;
    font->Shutdown();
    r->Shutdown();*/
}

