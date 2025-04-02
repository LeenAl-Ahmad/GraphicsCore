#include "PhysicsController.h"
#include "Renderer.h"
#include "Particle.h"
#include "RigidBody.h"

PhysicsController::PhysicsController()
{
    m_gravity = -9.81f; // Earth's gravitational pull constant
    m_force = glm::vec2{0, 0};
    Particle::Pool = new ObjectPool<Particle>();
    RigidBody::Pool = new ObjectPool<RigidBody>();
}

PhysicsController::~PhysicsController()
{
    delete Particle::Pool;
    delete RigidBody::Pool;
}

void PhysicsController::Reflect(glm::vec2& _o, const glm::vec2& _i, const glm::vec2& _n)
{
    _o = _i - 2.0f * glm::dot(_i, _n) * _n;
}

void PhysicsController::HandleCollision(RigidBody* _b1, RigidBody* _b2, float _distance)
{
    glm::vec2 x = _b1->GetPosition() - _b2->GetPosition(); // Collision Direction
    glm::vec2 v = _b1->GetVelocity() - _b2->GetVelocity(); // Movement Direction

    glm::vec2 v1 = _b1->GetVelocity();
    v1 -= (2 * _b2->GetMass() / (_b1->GetMass() + _b2->GetMass())) *
        (glm::dot(v, x) / ((float)pow(glm::length(x), 2))) * x;
    _b1->SetVelocity(v1);

    x *= -1;
    v = _b2->GetVelocity();
    glm::vec2 v2 = _b2->GetVelocity();
    v2 -= (2 * _b1->GetMass() / (_b1->GetMass() + _b2->GetMass())) *
        (glm::dot(v, x) / ((float)pow(glm::length(x), 2))) * x;
    _b2->SetVelocity(v2);
}


void PhysicsController::Update(float _deltaTime)
{
    for (int count = 0; count < m_particles.size(); count++)
    {
        Particle* p = m_particles[count];
        m_force.y -= p->GetMass() * m_gravity;
        p->Update(_deltaTime, m_force);
        if (!p->GetDead()) continue;
        p->Reset();
        Particle::Pool->ReleaseResource(p);
        m_particles.erase(m_particles.begin() + count);
        count--;
    }

    Point p = Renderer::Instance().GetWindowSize();
    glm::vec2 out;

    for (int count = 0; count < m_bodies.size(); count++)
    {
        RigidBody* b = m_bodies[count];
        b->Update(_deltaTime, glm::vec2(0, 0));

        if (b->GetPosition().x <= 16)
        {
            Reflect(out, b->GetVelocity(), glm::vec2(1, 0));
            b->SetVelocity(out);
        }
        else if (b->GetPosition().x >= p.X - 16)
        {
            Reflect(out, b->GetVelocity(), glm::vec2(-1, 0));
            b->SetVelocity(out);
        }
        else if (b->GetPosition().y <= 16)
        {
            Reflect(out, b->GetVelocity(), glm::vec2(0, 1));
            b->SetVelocity(out);
        }
        else if (b->GetPosition().y >= p.Y - 16)
        {
            Reflect(out, b->GetVelocity(), glm::vec2(0, -1));
            b->SetVelocity(out);
        }
    }
    // See if there are any intersections
    if (m_bodies.empty()) return;

    for (int c1 = 0; c1 < m_bodies.size() - 1; c1++)
    {
        for (int c2 = c1 + 1; c2 < m_bodies.size(); c2++)
        {
            RigidBody* b1 = m_bodies[c1];
            RigidBody* b2 = m_bodies[c2];
            float dist = glm::distance(b1->GetPosition(), b2->GetPosition());

            if (dist <= 32)
            {
                HandleCollision(b1, b2, dist);

                do
                {
                    b1->Update(_deltaTime, glm::vec2{ 0, 0 });
                    b2->Update(_deltaTime, glm::vec2{ 0, 0 });
                } while (glm::distance(b1->GetPosition(), b2->GetPosition()) <= 32);
            }
        }
    }

}


Particle* PhysicsController::AddParticle(glm::vec2 _position, float _lifeTime)
{
    Particle* particle = Particle::Pool->GetResource();
    particle->SetPosition(_position);
    particle->SetLifeTime(_lifeTime);
    particle->SetMass(1);
    m_particles.push_back(particle);
    return particle;
}

string PhysicsController::ToString()
{
    string retVal = "Particles: ";
    retVal += to_string(m_particles.size());
    retVal += "  RigidBodies:  ";
    retVal += to_string(m_bodies.size());
    return retVal;
}

RigidBody* PhysicsController::AddRigidBody(glm::vec2 _pos, glm::vec2 _dir, float _mass)
{
    RigidBody* body = RigidBody::Pool->GetResource();
    body->SetPosition(_pos);
    body->SetVelocity(_dir);
    body->SetMass(_mass);
    m_bodies.push_back(body);
    return body;;
}