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

void Unit::AssignNonDefaultValues() {
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

void Unit::HandleInput(SDL_Event event) {
    InputController* input = &InputController::Instance();
    velocity = glm::vec2(0);
    isMoving = false;

    if (input->KB()->KeyDown(event, SDLK_d)) {
        velocity.x = -moveSpeed;
        facingRight = false;
        isMoving = true;
    }
    if (input->KB()->KeyDown(event, SDLK_a)) {
        velocity.x = moveSpeed;
        facingRight = true;
        isMoving = true;
    }
    if (input->KB()->KeyDown(event, SDLK_w)) {
        velocity.y = -moveSpeed;
        isMoving = true;
    }
    if (input->KB()->KeyDown(event, SDLK_s)) {
        velocity.y = moveSpeed;
        isMoving = true;
    }

    // Normalize diagonal movement
    if (velocity.x != 0 && velocity.y != 0) {
        velocity = glm::normalize(velocity) * moveSpeed;
    }
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