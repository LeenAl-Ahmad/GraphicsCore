#ifndef PLAYER_H
#define PLAYER_H

#include "PlayerState.h"
#include "RigidBody.h"

class SpriteSheet;

class Player
{
public:
    // Constructors / Destructors
    Player();
    virtual ~Player();

    // Accessors
    PlayerState* GetState() { return m_state; }
    void SetState(PlayerState* _state) { m_state = _state; }
    SpriteSheet* GetAnimations() { return m_animations; }
    Rect* GetSrcRect() { return &m_srcRect; }
    RigidBody* GetRigidBody() { return m_rigidBody; }

    // Methods
    virtual void HandleInput(SDL_Event _event, float _deltaTime);
    virtual void Update(float _deltaTime);
    virtual void Render(Renderer* _renderer);

private:
    PlayerState* m_state;
    RigidBody* m_rigidBody;
    SpriteSheet* m_animations;
    Rect m_srcRect;
};

#endif // PLAYER_H
