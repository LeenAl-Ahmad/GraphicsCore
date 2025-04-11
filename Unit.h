#pragma once
#include "SpriteSheet.h"
#include "RigidBody.h"

class Unit {

public:
    Unit(SpriteSheet* sprite, RigidBody* body) :
        m_sprite(sprite), m_physicsBody(body) {
    }
    ~Unit();

    
    
    void Render(Renderer* renderer, Timing* timing) {
        if (!m_sprite || !renderer) return;

        auto pos = m_physicsBody->GetPosition();
        Rect r = m_sprite->Update(EN_AN_IDLE, timing->GetDeltaTime());

        renderer->RenderTexture(m_sprite, r,
            Rect(pos.x - 16, pos.y - 16, pos.x + 16, pos.y + 16),
            m_physicsBody->GetMass() + 127);
    }

    RigidBody* GetBody() { return m_physicsBody; }
    // Add other methods as needed

private:
    SpriteSheet* m_sprite;  // or Texture* if you prefer
    RigidBody* m_physicsBody;
    Texture* m_texture;

};