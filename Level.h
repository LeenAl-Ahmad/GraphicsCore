#ifndef LEVEL_H
#define LEVEL_H

#include "Renderer.h"
#include "Unito.h"
#include "Resource.h"
#include "Timing.h"
#include "PhysicsController.h"

class Level : public Resource
{
public:
    Level();
    ~Level();

    void AssignNonDefaultValues();
    void Render(Renderer* renderer, Timing* timing);
    

private:
    bool m_quit;
    Unit* m_playerUnit;
    SpriteSheet* m_circle;
    std::vector<Unit*> m_units;
    Unit* warrior;
    Timing* timing;
    PhysicsController* m_physics;

    
};

#endif // LEVEL_H