#ifndef RENDER_TARGET_H
#define RENDER_TARGET_H

#include "StandardIncludes.h"

class RenderTarget
{
public:
	RenderTarget();
	~RenderTarget();

	void Create(unsigned int _w, unsigned int _h);
	void Start();
	void stop();
	void Render(float _deltaTime);

private:
	SDL_Texture* m_texture;
	unsigned int m_width;
	unsigned int m_height;
	float m_rotation;
};

#endif // !RENDER_TARGET_H