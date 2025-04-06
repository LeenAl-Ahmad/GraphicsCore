#ifndef UNIT_H
#define UNIT_H

#include "SpriteSheet.h"
#include "SoundEffect.h"
#include "RigidBody.h"
#include "Timing.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "AudioController.h"
#include "InputController.h"

class Unit : public RigidBody
{
public:
    Unit();
    ~Unit();

    void AssignNonDefValues();
    Rect Update(AnimationNames _name, float _deltaTime);
    void UpdateMovement(float deltaTime);
    void Serialize(std::ostream& _stream);
    void Deserialize(std::istream& _stream);

    SpriteSheet* GetWarrior() { return warrior; }
    glm::vec2 GetPosition() const { return position; }
    void SetPosition(const glm::vec2& pos) { position = pos; }
    float GetScale() const { return scale; }
    void SetScale(float _s) { this->scale = _s;  }
    bool IsMoving() const { return isMoving; }

    bool IsFacingRight() const { return facingRight; }
    void SetFacingRight(bool facing) { facingRight = facing; }

    static ObjectPool<Unit>* Pool;


    enum State { ALIVE = 0, DYING = 1, DEAD = 2 };

    // Animation control
    void UpdateAnimation(float deltaTime, float gameTime);
    void PlayDeathAnimation();
    Rect GetCurrentFrame() const;

    // Animation tracking
    State state = ALIVE;
    SpriteAnim* runAnim = nullptr;
    SpriteAnim* deathAnim = nullptr;
    float deathAnimationTime = 0.0f;

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