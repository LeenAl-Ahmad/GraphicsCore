#include "GameController.h"
#include "Renderer.h"

GameController::GameController()
{
    m_sdlEvent = { }; 
}

GameController::~GameController()
{
}

void GameController::RunGame()
{
    AssetController::Instance().Initialize(10000000);
    Renderer* r = &Renderer::Instance();
    r->Initialize(800, 600);
    Point ws = r->GetWindowSize();

    Texture::Pool = new ObjectPool<Texture>();
    Texture* texture = Texture::Pool->GetResource();
    texture->Load("C:/Users/leana/source/repos/GraphicsCore/Assets/Textures/Emoji.tga");

    while (m_sdlEvent.type != SDL_QUIT)
    {
        SDL_PollEvent(&m_sdlEvent);
        r->SetViewPort(Rect(0, 0, ws.X, ws.Y));
        r->SetDrawColor(Color(255, 0, 0, 255));
        r->ClearScreen();
        r->SetViewPort(Rect(0, 0, ws.X/2, ws.Y/2));
        r->RenderTexture(texture, Point(0, 0));
        r->SetViewPort(Rect(ws.X/2, 0, ws.X, ws.Y / 2));
        r->RenderTexture(texture, Rect(0, 0, ws.X / 2, ws.Y / 2));
        r->SetViewPort(Rect(0, ws.Y/2, ws.X/2, ws.Y));
        r->RenderTexture(texture, Rect(0, 0, ws.X / 2, ws.Y / 2));
        r->SetViewPort(Rect(ws.X / 2, ws.Y/2, ws.X, ws.Y));
        r->RenderTexture(texture, Point(0, 0));
        SDL_RenderPresent(r->GetRenderer());
    }
    delete Texture::Pool;
    r->Shutdown();
}
