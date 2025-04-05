#include "Unito.h"

ObjectPool<Unit>* Unit::Pool = nullptr;

Unit::Unit()
{
    warrior = nullptr;
}

Unit::~Unit() {
    if (SpriteAnim::Pool != nullptr)
    {
        delete SpriteAnim::Pool;
        SpriteAnim::Pool = nullptr;
    }
    if (SpriteSheet::Pool != nullptr)
    {
        delete SpriteSheet::Pool;
        SpriteAnim::Pool = nullptr;
    }
}

void Unit::AssignNonDefaultValues() {
    SpriteSheet::Pool = new ObjectPool<SpriteSheet>();
    SpriteAnim::Pool = new ObjectPool<SpriteAnim>();

    warrior = SpriteSheet::Pool->GetResource();
    warrior->Load("C:/Users/leana/source/repos/GraphicsCore/Assets/Textures/Warrior.tga");
    warrior->SetSize(17, 6, 69, 44);
    warrior->AddAnimation(EN_AN_IDLE, 0, 7, 0.2f);
    warrior->SetBlendMode(SDL_BLENDMODE_BLEND);
}

Rect Unit::Update(AnimationNames _name, float _deltaTime)
{
    return warrior->Update(_name, _deltaTime);
}
/*void Unit::SetAnimation(AnimationNames name) {
    m_currentAnim = name;
}

Rect Unit::GetCurrentFrame() const {
    if (m_spriteSheet) {
        return m_spriteSheet->Update(m_currentAnim, Timing::Instance().GetDeltaTime());
    }
    return Rect{ 0, 0, 32, 32 }; // Default frame
}

void Unit::Update(float deltaTime) {

    m_spriteSheet->Update(m_currentAnim, deltaTime);
}

void Unit::Serialize(std::ostream& stream) {
    Resource::Serialize(stream);

    // Serialize basic types
    stream.write(reinterpret_cast<const char*>(&position), sizeof(glm::vec2));
    stream.write(reinterpret_cast<const char*>(&velocity), sizeof(glm::vec2));
    stream.write(reinterpret_cast<const char*>(&m_health), sizeof(int));
    stream.write(reinterpret_cast<const char*>(&m_currentAnim), sizeof(AnimationNames));

    // Serialize SpriteSheet
    byte hasSpriteSheet = (m_spriteSheet != nullptr);
    stream.write(reinterpret_cast<const char*>(&hasSpriteSheet), sizeof(byte));
    if (hasSpriteSheet) {
        m_spriteSheet->Serialize(stream);
    }
}

void Unit::Deserialize(std::istream& stream) {
    Resource::Deserialize(stream);

    // Deserialize basic types
    stream.read(reinterpret_cast<char*>(&position), sizeof(glm::vec2));
    stream.read(reinterpret_cast<char*>(&velocity), sizeof(glm::vec2));
    stream.read(reinterpret_cast<char*>(&m_health), sizeof(int));
    stream.read(reinterpret_cast<char*>(&m_currentAnim), sizeof(AnimationNames));

    // Deserialize SpriteSheet
    byte hasSpriteSheet;
    stream.read(reinterpret_cast<char*>(&hasSpriteSheet), sizeof(byte));
    if (hasSpriteSheet) {
        m_spriteSheet = SpriteSheet::Pool->GetResource();
        m_spriteSheet->Deserialize(stream);
    }

}

void Unit::ToString() {
    cout << "UNIT" << endl;
    cout << "Position: (" << position.x << ", " << position.y << ")" << endl;
    cout << "Velocity: (" << velocity.x << ", " << velocity.y << ")" << endl;
    cout << "Health: " << m_health << endl;
    cout << "Current Animation: " << m_currentAnim << endl;

    // Call base class ToString
    Resource::ToString();
}*/
