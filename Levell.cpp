#include "Levell.h"

Level::Level()
    : m_quit(false),
    m_playerUnit(nullptr),
    m_circle(nullptr)
{
    // Initialize object pools
    Unit::Pool = new ObjectPool<Unit>();
    SpriteSheet::Pool = new ObjectPool<SpriteSheet>();
    SpriteAnim::Pool = new ObjectPool<SpriteAnim>();

    AssignNonDefaultValues();
}

Level::~Level() {
    // Clean up object pools
    delete Unit::Pool;
    delete SpriteSheet::Pool;
    delete SpriteAnim::Pool;

    // Clean up units
    for (auto unit : m_units) {
        Unit::Pool->ReturnResource(unit);
    }

    if (m_circle) {
        SpriteSheet::Pool->ReturnResource(m_circle);
    }
}

void Level::AssignNonDefaultValues() {
    // Create player unit
    m_playerUnit = Unit::Pool->GetResource();
    m_playerUnit->AssignNonDefaultValues();
    m_units.push_back(m_playerUnit);

    // Load circle sprite
    m_circle = SpriteSheet::Pool->GetResource();
    m_circle->Load("./Assets/Textures/Circle.tga");
    m_circle->SetSize(1, 1, 32, 32);
    m_circle->AddAnimation(EN_AN_IDLE, 0, 1, 0.0f);
    m_circle->SetBlendMode(SDL_BLENDMODE_BLEND);
}


void Level::Update(float deltaTime) {
    const float fixedDelta = 1.0f / 60.0f; // 60 FPS physics
    int steps = std::ceil(deltaTime / fixedDelta);

    for (int i = 0; i < steps; ++i) {
        for (auto& unit : m_units) {
            if (unit) unit->Update(fixedDelta);
        }
    }
}

void Level::Render(Renderer* renderer) {
    if (!renderer) return;

    renderer->SetDrawColor(Color(255, 255, 255, 255));
    renderer->ClearScreen();

    // Render all units
    Rect r = m_circle->Update(EN_AN_IDLE, Timing::Instance().GetDeltaTime());
    for (Unit* unit : m_units) {
        auto pos = unit->position;
        renderer->RenderTexture(m_circle, r, Rect(pos.x - 16, pos.y - 16, pos.x + 16, pos.y + 16), 255);
    }
}

void Level::HandleInput(SDL_Event event) {
    auto input = InputController::Instance();

    if ((event.type == SDL_QUIT) || (input.KB()->KeyUp(event, SDLK_ESCAPE))) {
        m_quit = true;
    }
    else if (input.KB()->KeyDown(event, SDLK_a)) {
        // Add new unit 
        Unit* newUnit = Unit::Pool->GetResource();
        newUnit->AssignNonDefaultValues();
        newUnit->position = glm::vec2{ 16 + rand() % (1920 - 32), 16 + rand() % (1080 - 32) };
        m_units.push_back(newUnit);
    }
}

void Level::Serialize(std::ostream& stream) {
    _stream.write(reinterpret_cast<const char*>(&position), sizeof(position));
    _stream.write(reinterpret_cast<const char*>(&velocity), sizeof(velocity));
    Resource::Serialize(stream);
}

void Level::Deserialize(std::istream& stream) {
    _stream.read(reinterpret_cast<const char*>(&position), sizeof(position));
    _stream.read(reinterpret_cast<const char*>(&velocity), sizeof(velocity));
    Resource::Deserialize(stream);
}

void Level::ToString() {
    cout << "LEVEL - Units: " << m_units.size() << endl;
    Resource::ToString();
}