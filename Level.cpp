
#include "Level.h"

Level::Level()
{
    m_physics = nullptr;
    warrior = nullptr;
    
}

Level::~Level() {
    for (auto unit : m_units) {
        Unit::Pool->ReturnResource(unit);
    }
    if (m_circle) {
        SpriteSheet::Pool->ReturnResource(m_circle);
        m_circle = nullptr;
    }
}

void Level::AssignNonDefaultValues() {
    m_physics = &PhysicsController::Instance();
    timing = &Timing::Instance();
    Unit::Pool = new ObjectPool<Unit>();
    SpriteSheet::Pool = new ObjectPool<SpriteSheet>();
    SpriteAnim::Pool = new ObjectPool<SpriteAnim>();

    warrior = Unit::Pool->GetResource();
    warrior->AssignNonDefaultValues();
}

void Level::Render(Renderer* renderer, Timing* timing) {

    renderer->SetDrawColor(Color(255, 255, 255, 255));
    renderer->ClearScreen();

    m_physics->Update(timing->GetDeltaTime());
    Level::AssignNonDefaultValues();
    renderer->RenderTexture(warrior->GetWarrior(), warrior->Update(EN_AN_IDLE, timing->GetDeltaTime()), Rect(300, 200, 400, 300));
}
/*Unit* Level::CreateUnit(glm::vec2 position) {
    Unit* unit = Unit::Pool->GetResource();
    if (unit) {
        unit->position = position;
        unit->AssignNonDefaultValues();
        m_units.push_back(unit);
    }
    return unit;
}

void Level::Update(float deltaTime) {
    const float fixedDelta = 1.0f / 60.0f;
    int steps = std::ceil(deltaTime / fixedDelta);
    for (int i = 0; i < steps; ++i) {
        for (auto& unit : m_units) {
            if (unit) unit->Update(fixedDelta);
        }
    }
}

void Level::Serialize(std::ostream& stream) {
    stream.write(reinterpret_cast<const char*>(&m_quit), sizeof(m_quit));
    size_t count = m_units.size();
    stream.write(reinterpret_cast<const char*>(&count), sizeof(count));
    for (auto unit : m_units) {
        unit->Serialize(stream);
    }
    Resource::Serialize(stream);
}

void Level::Deserialize(std::istream& stream) {
    stream.read(reinterpret_cast<char*>(&m_quit), sizeof(m_quit));
    size_t count;
    stream.read(reinterpret_cast<char*>(&count), sizeof(count));
    m_units.resize(count);
    for (size_t i = 0; i < count; ++i) {
        m_units[i] = Unit::Pool->GetResource();
        m_units[i]->Deserialize(stream);
    }
    Resource::Deserialize(stream);
}

void Level::ToString() {
    cout << "LEVEL - Units: " << m_units.size() << endl;
    Resource::ToString();
}*/
