#ifndef RENDERER_H
#define RENDERER_H

#include "Texture.h"

class Asset;

class Renderer : public Singleton<Renderer>
{
    public:
        // Constructors/Destructors
        Renderer();
        virtual ~Renderer();

        // Accessors
        SDL_Window* GetWindow() { return m_window; }
        SDL_Renderer* GetRenderer() { return m_renderer; }
        SDL_Texture* GetSDLTexture(Texture* _t);
        vector<SDL_DisplayMode>& GetResolutions() { return m_resolutions; }

        // Methods
        void Initialize();
        void ChangeDisplayMode(SDL_DisplayMode* _m);
        void EnumerateDisplayMode();
        Point GetWindowSize();
        void SetDrawColor(Color _color);
        void ClearScreen();
        void SetViewPort(Rect _view);
        void RenderPoint(Point _p);
        void RenderLine(Rect _p);
        void RenderRectangle(Rect _rect);
        void RenderFillRectangle(Rect _rect);
        void RenderTexture(Texture* _t, Point _p);
        void RenderTexture(Texture* _texture, Rect _rect);
        void RenderTexture(Texture* _texture, Rect _srect, Rect _destRect);
        void RenderTexture(SDL_Texture* _texture, Rect _srect, Rect _destRect, double _angle);
        void Shutdown();

    private:
        // Members
        SDL_Window* m_window;
        SDL_Renderer* m_renderer;
        SDL_Rect m_destRect;
        SDL_Rect m_srect;
        SDL_Surface* m_surface;
        SDL_Rect m_viewPort;
        map<string, SDL_Texture*> m_texture;
        vector<SDL_DisplayMode> m_resolutions;
    };

#endif // RENDERER_H

