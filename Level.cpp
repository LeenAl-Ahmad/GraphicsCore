#include "Level.h"

Level::Level()
    : m_quit(false),
    m_playerUnit(nullptr),
    m_circle(nullptr)
{
    Unit::Pool = new ObjectPool<Unit>();
    SpriteSheet::Pool = new ObjectPool<SpriteSheet>();
    SpriteAnim::Pool = new ObjectPool<SpriteAnim>();
    AssignNonDefaultValues();
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
    // Create player unit with specific animations
    m_playerUnit = Unit::Pool->GetResource();
    if (m_playerUnit && m_playerUnit->GetSpriteSheet()) {
        m_playerUnit->GetSpriteSheet()->Load("./Assets/Textures/Player.tga");
        m_playerUnit->GetSpriteSheet()->SetSize(6, 8, 64, 64);
        m_playerUnit->GetSpriteSheet()->AddAnimation(EN_AN_IDLE, 0, 4, 0.2f);
        m_playerUnit->GetSpriteSheet()->AddAnimation(EN_AN_RUN, 8, 6, 0.1f);
        m_playerUnit->position = glm::vec2(100, 100);
        m_units.push_back(m_playerUnit);
    }

    // Create enemies if level 1
        for (int i = 0; i < 10; ++i) {
            Unit* enemy = Unit::Pool->GetResource();
            if (enemy && enemy->GetSpriteSheet()) {
                enemy->GetSpriteSheet()->Load("./Assets/Textures/Warrior.tga");
                enemy->GetSpriteSheet()->SetSize(17, 6, 69, 44);
                enemy->GetSpriteSheet()->AddAnimation(EN_AN_RUN, 6, 8, 0.00008f);
                
                enemy->position = glm::vec2(0, 10 + i * 100);
                enemy->velocity = glm::vec2(80 + (rand() % 21), 0);
                enemy->SetAnimation(EN_AN_RUN);
                m_units.push_back(enemy);
            }
        }
}

void Level::Render(Renderer* renderer) {
    if (!renderer) return;

    renderer->SetDrawColor(Color(255, 255, 255, 255));
    renderer->ClearScreen();

    for (Unit* unit : m_units) {
        if (unit) {
            Rect frame = unit->GetCurrentFrame();
            auto pos = unit->position;
            renderer->RenderTexture(unit->GetSpriteSheet(), frame,
                Rect{ static_cast<unsigned int>(pos.x - 16),
                     static_cast<unsigned int>(pos.y - 16),
                     32, 32 },
                255);
        }
    }
}

Unit* Level::CreateUnit(glm::vec2 position) {
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
}