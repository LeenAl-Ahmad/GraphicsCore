#include "Unito.h"

ObjectPool<Unit>* Unit::Pool = nullptr;

Unit::Unit()
{
	m_audio = nullptr;
	memset(m_effects, 0, sizeof(SoundEffect*) * MaxEffectChannels);
}

Unit::~Unit() {
	for (int i = 0; i < MaxEffectChannels; i++) {
		if (m_effects[i]) {
			// Properly release sound effects
			delete m_effects[i];
			m_effects[i] = nullptr;
		}
	}
}

void Unit::AssignNonDefaultValues() {
    m_audio = &AudioController::Instance();

    if (m_spriteSheet) {
        m_spriteSheet->Load("./Assets/Textures/Warrior.tga");
        m_spriteSheet->SetSize(1, 1, 32, 32);
        m_spriteSheet->AddAnimation(EN_AN_IDLE, 0, 1, 0.0f);
        m_spriteSheet->SetBlendMode(SDL_BLENDMODE_BLEND);
    }
}

void Unit::SetAnimation(AnimationNames name) {
    m_currentAnim = name;
}

Rect Unit::GetCurrentFrame() const {
    if (m_spriteSheet) {
        return m_spriteSheet->Update(m_currentAnim, Timing::Instance().GetDeltaTime());
    }
    return Rect{ 0, 0, 32, 32 }; // Default frame
}

void Unit::Update(float deltaTime) {
    if (m_spriteSheet) {
        m_spriteSheet->Update(m_currentAnim, deltaTime);
    }

    // Update audio effects positions
    if (m_audio) {
        for (int i = 0; i < MaxEffectChannels; ++i) {
            if (m_effects[i]) {
                
            }
        }
    }
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

    // Serialize SoundEffects
    for (int i = 0; i < MaxEffectChannels; ++i) {
        byte hasEffect = (m_effects[i] != nullptr); 
        stream.write(reinterpret_cast<const char*>(&hasEffect), sizeof(byte));
        if (hasEffect) {
            m_effects[i]->Serialize(stream);
        }
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

    // Deserialize SoundEffects
    for (int i = 0; i < MaxEffectChannels; ++i) {
        byte hasEffect;
        stream.read(reinterpret_cast<char*>(&hasEffect), sizeof(byte));
        if (hasEffect) {
            m_effects[i] = SoundEffect::Pool->GetResource();
            m_effects[i]->Deserialize(stream);
        }
    }

    // Recreate AudioController reference
    m_audio = &AudioController::Instance();
}

void Unit::ToString() {
    cout << "UNIT" << endl;
    cout << "Position: (" << position.x << ", " << position.y << ")" << endl;
    cout << "Velocity: (" << velocity.x << ", " << velocity.y << ")" << endl;
    cout << "Health: " << m_health << endl;
    cout << "Current Animation: " << m_currentAnim << endl;

    // Call base class ToString
    Resource::ToString();
}