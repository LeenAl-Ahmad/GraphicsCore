#include "Unit.h"

Unit::Unit(SpriteSheet* sprite, const glm::vec2& position) :
    m_sprite(sprite)
{
    // Initialize physics body
    m_physicsBody = new RigidBody(position);

    // Configure sprite if needed
    if (m_sprite) {
        m_sprite->SetBlendMode(SDL_BLENDMODE_BLEND);
    }
}

Unit::~Unit() {
    if (m_physicsBody) delete m_physicsBody;
    // Note: Don't delete m_sprite here - it's managed by the pool
}



