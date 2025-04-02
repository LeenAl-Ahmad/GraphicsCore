#include "TTFont.h"
#include "Renderer.h"

TTFont::TTFont()
{
    m_font = nullptr;
    destRect = { };
}

TTFont::~TTFont()
{
    Shutdown();
}

void TTFont::Initialize(int _pointSize)
{
    M_ASSERT((TTF_Init() >= 0), "Unable to initialize SDL TTF.");
    M_ASSERT((m_font = TTF_OpenFont("./Assets/Fonts/arial.ttf", _pointSize)) != nullptr, "Failed to load font.");
}

void TTFont::Shutdown()
{
    TTF_Quit();
}

void TTFont::Write(SDL_Renderer* _renderer, const char* _text, SDL_Color _color, SDL_Point _pos)
{
    if (strlen(_text) == 0) return;

    SDL_Surface* surface = TTF_RenderUTF8_Blended(m_font, _text, _color);
    if (!surface) {
        fprintf(stderr, "TTF_RenderUTF8_Blended failed: %s\n", TTF_GetError());
        return;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(_renderer, surface);
    

    SDL_Rect destRect = { _pos.x, _pos.y, surface->w, surface->h };
    int renderResult = SDL_RenderCopyEx(_renderer, texture, nullptr, &destRect, 0, nullptr, SDL_FLIP_NONE);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}
