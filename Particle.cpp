#include "Particle.h"

ObjectPool<Particle>* Particle::Pool;

Particle::Particle()
{
	Reset();
}

void Particle::Reset()
{
	m_currentSize = 0;
	m_currentTime = 0;
	m_startSize = 0;
	m_endSize = 1;
	m_lifeTime = 0;
	RigidBody::Reset();
}

void Particle::Update(float _dT, glm::vec2 _f)
{
	RigidBody::Update(_dT, _f);

	float sizePerc = m_currentTime / m_lifeTime;
	m_currentSize = m_startSize + ((m_endSize - m_startSize) * sizePerc);

	m_currentTime += _dT;
	if (m_currentTime > m_lifeTime)
	{
		SetDead(true);
	}
}