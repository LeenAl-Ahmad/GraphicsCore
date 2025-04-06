#include "Unito.h"

ObjectPool<Unit>* Unit::Pool = nullptr;

Unit::Unit()
{
    warrior = nullptr;
    position = glm::vec2(0);
    velocity = glm::vec2(0);
}

Unit::~Unit() {
    if (warrior) {
        SpriteSheet::Pool->ReturnResource(warrior);
    }
}

void Unit::AssignNonDefValues() {
    warrior = SpriteSheet::Pool->GetResource();
    warrior->Load("C:/Users/leana/source/repos/GraphicsCore/Assets/Textures/Warrior.tga");
    warrior->SetSize(17, 6, 69, 44);
    warrior->AddAnimation(EN_AN_IDLE, 0, 7, 0.2f);
    warrior->AddAnimation(EN_AN_RUN, 8, 8, 0.15f); // Assuming run animation starts at clip 8
    warrior->SetBlendMode(SDL_BLENDMODE_BLEND);
    // blue tint in progress
}

Rect Unit::Update(AnimationNames _name, float _deltaTime) {
    currentAnim = _name;
    return warrior->Update(_name, _deltaTime);
}

void Unit::UpdateMovement(float deltaTime) {
    if (isMoving) {
        position += velocity * deltaTime;
        currentAnim = EN_AN_RUN;
    }
    else {
        currentAnim = EN_AN_IDLE;
    }

    // Set rotation based on facing direction
    rotation = facingRight ? 0.0f : 180.0f;
}