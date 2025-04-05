#ifndef UNIT_H
#define UNIT_H

#include "SpriteSheet.h"
#include "SoundEffect.h"
#include "Timing.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "AudioController.h"
#include "InputController.h"

class Unit : public Texture
{
public:
    Unit();
    ~Unit();

    void AssignNonDefaultValues() override;
    Rect Update(AnimationNames _name, float _deltaTime);
    void HandleInput(SDL_Event event);
    void UpdateMovement(float deltaTime);

    SpriteSheet* GetWarrior() { return warrior; }
    glm::vec2 GetPosition() const { return position; }
    void SetPosition(const glm::vec2& pos) { position = pos; }
    float GetScale() const { return scale; }
    bool IsMoving() const { return isMoving; }

    static ObjectPool<Unit>* Pool;

    // Transform
    glm::vec2 position;
    glm::vec2 velocity;
    float rotation = 0.0f;
    float scale = 1.25f;
    bool facingRight = true;

private:
    InputController* input;
    SpriteSheet* warrior;
    AnimationNames currentAnim = EN_AN_IDLE;
    bool isMoving = false;
    const float moveSpeed = 100.0f; // units per second
};
#endif