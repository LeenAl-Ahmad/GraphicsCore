#ifndef LEVEL_H
#define LEVEL_H

#include "Resource.h"
#include "Unito.h"
#include "Renderer.h"
#include "SpriteSheet.h"
#include <vector>
#include "Keyboard.h"
#include "Mouse.h"
#include "AssetController.h"
#include "InputController.h"
#include "PhysicsController.h"
#include "Timing.h"

class Level : public Resource {
public:
    Level();
    virtual ~Level();

    // Resource overrides
    void Serialize(std::ostream& stream) override;
    void Deserialize(std::istream& stream) override;
    void AssignNonDefaultValues() override;
    void ToString() override;

    // Game methods
    void Update(float deltaTime);
    void Render(Renderer* renderer);
    Unit* CreateUnit(glm::vec2 position);
    bool ShouldQuit() const { return m_quit; }

private:
    bool m_quit = false;
    SpriteSheet* sheet;
    Unit* m_playerUnit = nullptr;
    std::vector<Unit*> m_units;
    SpriteSheet* m_circle;
};
#endif