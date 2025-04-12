#include "RigidBody.h"

ObjectPool<RigidBody>* RigidBody::Pool;

RigidBody::RigidBody()
{
    Reset();
}

void RigidBody::Reset()
{
    m_position = { };
    m_velocity = { };
    m_acceleration = { };
    m_buoyancy = { };
    m_buoyancyDecay = { };
    m_buoyancyMin = { 0, 0};
    m_randomForce = { 0, 0 };
    m_wind = { };
    m_mass = 0;
    m_dead = false;
}

void RigidBody::Update(float _deltaTime, glm::vec2 _force)
{
    // Apply forces
    _force += m_wind + m_buoyancy + m_randomForce;
    _force.y *= -1; // Invert force y direction, since the rendering y-axis is inverted
    m_acceleration = _force * m_mass;
    m_velocity += m_acceleration * _deltaTime;
    m_position += m_velocity * _deltaTime;

    // Calculate decays
    m_buoyancy -= m_buoyancyDecay * _deltaTime;
    m_buoyancy.x = std::fmax(m_buoyancy.x, m_buoyancyMin.x);
    m_buoyancy.y = std::fmax(m_buoyancy.y, m_buoyancyMin.y);
}
