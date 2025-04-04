#pragma once
#include "RigidBody.h"
#include "SpriteSheet.h"
#include "Renderer.h"

class Unit {
public:
    // Constructor with built-in sprite sheet setup
    Unit(const std::string& texturePath,
        int rows, int cols,
        int frameWidth, int frameHeight);

    void Update(float deltaTime);
    void Render();

    // Movement
    void SetPosition(float x, float y);
    void SetVelocity(float x, float y);

    // Visuals
    void SetTint(Uint8 r, Uint8 g, Uint8 b);
    void AddAnimation(AnimationNames name, int startFrame, int frameCount, float speed);
    void PlayAnimation(AnimationNames name);


private:
    RigidBody m_body;
    SpriteSheet* m_spriteSheet;
    SDL_Color m_tint;
    AnimationNames m_currentAnim;
};