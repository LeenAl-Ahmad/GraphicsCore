#ifndef MOUSE_H
#define MOUSE_H

#include "StandardIncludes.h"

class Mouse
{
public:
	Mouse();
	virtual ~Mouse();

	Point* GetPos() { return &m_pos; }
	Point* GetPosPrev() { return &m_posPrev; }
	bool GetButLDown() { return m_butLDown; }
	bool GetButMDown() { return m_butMDown; }
	bool GetButRDown() { return m_butRDown; }

	bool Moved(SDL_Event _event, Point& _pos);
	void ProcessButtons(SDL_Event _event);

private:
	Point m_pos;
	Point m_posPrev;
	bool m_butLDown;
	bool m_butMDown;
	bool m_butRDown;
};

#endif // !MOUSE_H



