#include "Renderer.h"
#include "Asset.h"

Renderer::Renderer()
{
    m_window = nullptr;
    m_renderer = nullptr;
    m_destRect = { };
    m_srect = { };
    m_surface = nullptr;
}

Renderer::~Renderer()
{
    Shutdown();
}

void Renderer::Shutdown()
{
    for (auto it = m_texture.begin(); it != m_texture.end(); it++)
    {
        SDL_DestroyTexture(it->second);
    }
    m_texture.clear();
    if (m_renderer != nullptr)
    {
        SDL_DestroyRenderer(m_renderer);
        m_renderer = nullptr;
    }
    if (m_window != nullptr)
    {
        SDL_DestroyWindow(m_window);
        m_window = nullptr;
    }
    SDL_Quit(); // Quit SDL subsystems
}

void Renderer::SetDrawColor(Color _color)
{
    SDL_SetRenderDrawColor(m_renderer, _color.R, _color.G, _color.B, _color.A);
}

void Renderer::ClearScreen()
{
    SDL_RenderClear(m_renderer);
}

void Renderer::RenderPoint(Point _position)
{
    SDL_RenderDrawPoint(m_renderer, _position.X, _position.Y);
}

void Renderer::RenderLine(Rect _points)
{
    SDL_RenderDrawLine(m_renderer, _points.X1, _points.Y1, _points.X2, _points.Y2);
}

void Renderer::RenderRectangle(Rect _rect)
{
    m_destRect.x = _rect.X1;
    m_destRect.y = _rect.Y1;
    m_destRect.w = _rect.X2 - _rect.X1;
    m_destRect.h = _rect.Y2 - _rect.Y1;
    SDL_RenderDrawRect(m_renderer, &m_destRect);
}

void Renderer::RenderFillRectangle(Rect _rect)
{
    m_destRect.x = _rect.X1;
    m_destRect.y = _rect.Y1;
    m_destRect.w = _rect.X2 - _rect.X1;
    m_destRect.h = _rect.Y2 - _rect.Y1;
    SDL_RenderFillRect(m_renderer, &m_destRect);
}

SDL_Texture* Renderer::GetSDLTexture(Texture* _texture)
{
    // If texture has already been created, return created texture
    Asset* asset = _texture->GetData();
    string guid = asset->GetGUID();
    if (m_texture.count(guid) == 0)
    {
        // Otherwise, create the GPU texture
        ImageInfo* ii = _texture->GetImageInfo();
        m_surface = SDL_CreateRGBSurfaceFrom(asset->GetData() + _texture->GetImageInfo()->DataOffset,
        ii->Width, ii->Height, ii->BitsPerPixel,
        ii->Width * ii->BitsPerPixel / 8,
        0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);

        SDL_Texture* texture = SDL_CreateTextureFromSurface(m_renderer, m_surface);
        SDL_FreeSurface(m_surface);

        m_surface = nullptr;
        m_texture[guid] = texture; 
    }
    
    SDL_SetTextureBlendMode(m_texture[guid], _texture->GetBlendMode());
    SDL_SetTextureAlphaMod(m_texture[guid], _texture->GetBlendAlpha());
    return m_texture[guid];
}

void Renderer::RenderTexture(Texture* _texture, Point _point)
{
    m_destRect.x = _point.X;
    m_destRect.y = _point.Y;
    m_destRect.w = _texture->GetImageInfo()->Width;
    m_destRect.h = _texture->GetImageInfo()->Height;

    M_ASSERT((SDL_RenderCopyEx(m_renderer, GetSDLTexture(_texture),
        NULL, &m_destRect, 0, NULL, SDL_FLIP_VERTICAL)) >= 0, "Could not render texture");
}

Point Renderer::GetWindowSize()
{
    int w;
    int h;
    SDL_GetWindowSize(m_window, &w, &h);
    return Point(w, h);
}

void Renderer::SetViewPort(Rect _view)
{
    m_viewPort.x = _view.X1;
    m_viewPort.y = _view.Y1;
    m_viewPort.w = _view.X2 - _view.X1;
    m_viewPort.h = _view.Y2 - _view.Y1;
    SDL_RenderSetViewport(m_renderer, &m_viewPort);
}

void Renderer::RenderTexture(Texture* _texture, Rect _rect)
{
    m_destRect.x = _rect.X1;
    m_destRect.y = _rect.Y1;
    m_destRect.w = _rect.X2 - _rect.X1;
    m_destRect.h = _rect.Y2 - _rect.Y1;
    M_ASSERT(((SDL_RenderCopyEx(m_renderer, GetSDLTexture(_texture), 
        NULL, &m_destRect, 0, NULL, SDL_FLIP_VERTICAL)) >= 0),
        "Could not rendertexture");
}

void Renderer::RenderTexture(Texture* _texture, Rect _srect, Rect _rect, int _a)
{
    m_destRect.x = _rect.X1;
    m_destRect.y = _rect.Y1;
    m_destRect.w = _rect.X2 - _rect.X1;
    m_destRect.h = _rect.Y2 - _rect.Y1;

    m_srect.x = _srect.X1;
    m_srect.y = _texture->GetImageInfo()->Height - _srect.Y2;
    m_srect.w = _srect.X2 - _srect.X1;
    m_srect.h = _srect.Y2 - _srect.Y1;

    SDL_Texture* tex = GetSDLTexture(_texture);
    SDL_SetTextureAlphaMod(tex, _a);
    M_ASSERT(((SDL_RenderCopyEx(m_renderer, tex,
        &m_srect, &m_destRect, 0, nullptr, SDL_FLIP_VERTICAL)) >= 0),
        "Could not rendertexture");
}

void Renderer::RenderTexture(SDL_Texture* _texture, Rect _srect, Rect _rect, double _a)
{
    SDL_Point size;
    SDL_QueryTexture(_texture, NULL , NULL, &size.x, &size.y);

    m_destRect.x = _rect.X1;
    m_destRect.y = _rect.Y1;
    m_destRect.w = _rect.X2 - _rect.X1;
    m_destRect.h = _rect.Y2 - _rect.Y1;

    m_srect.x = _srect.X1;
    m_srect.y = _srect.Y2;
    m_srect.w = _srect.X2 - _srect.X1;
    m_srect.h = _srect.Y2 - _srect.Y1;

    M_ASSERT(((SDL_RenderCopyEx(m_renderer, _texture,
        &m_srect, &m_destRect, _a, nullptr, SDL_FLIP_NONE)) >= 0),
        "Could not render texture");
}

void Renderer::Initialize()
{
    M_ASSERT((SDL_Init(SDL_INIT_EVERYTHING) >= 0), "");
    SDL_GetDisplayBounds(0, &m_srect);
    m_window = SDL_CreateWindow("SDL Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
        m_srect.w, m_srect.y, SDL_WINDOW_FULLSCREEN);
    M_ASSERT(m_window != nullptr, "Failed to initialize SDL window.");
    m_renderer = SDL_CreateRenderer(Renderer::Instance().GetWindow(), -1, 0);
    M_ASSERT(m_renderer != nullptr, "Failed to intialize SDL renderer.");
}

void Renderer::EnumerateDisplayMode()
{
    int display_Count = SDL_GetNumVideoDisplays();
    for (int display_index = 0; display_index <= display_Count; display_index++)
    {
        int mode_count = SDL_GetNumDisplayModes(display_index);
        for (int mode_index=0; mode_index <= mode_count; mode_index++)
        {
            SDL_DisplayMode mode = { SDL_PIXELFORMAT_UNKNOWN, 0, 0, 0, 0 };
            if (SDL_GetDisplayMode(display_index, mode_index, &mode) == 0)
            {
                m_resolutions.push_back(mode);
            }
        }
    }
}

void Renderer::ChangeDisplayMode(SDL_DisplayMode* _m)
{
    M_ASSERT(SDL_SetWindowDisplayMode(m_window, _m) == 0, "Failed to set resolution");
    SDL_SetWindowSize(m_window, _m->w, _m->h);
}