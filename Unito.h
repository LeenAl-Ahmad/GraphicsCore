#ifndef UNIT_H
#define UNIT_H

#include "SpriteSheet.h"
#include "SoundEffect.h"
#include "Timing.h"
#include "AudioController.h"
#include "InputController.h"

class Unit : public Resource
{
public:
    Unit();
    ~Unit();

    // Resource overrides
    void Serialize(std::ostream& stream) override;
    void Deserialize(std::istream& stream) override;
    void ToString() override;
    void AssignNonDefaultValues() override;
    void Update(float deltaTime);

    void SetAnimation(AnimationNames name);
    Rect GetCurrentFrame() const;
    SpriteSheet* GetSpriteSheet() { return m_spriteSheet; }

    static ObjectPool<Unit>* Pool;

    // Transform
    glm::vec2 position;
    glm::vec2 velocity;

private:
    SpriteSheet* m_spriteSheet;
    AnimationNames m_currentAnim = EN_AN_IDLE;
    int m_health = 100;
    SoundEffect* m_effects[MaxEffectChannels];
    AudioController* m_audio;
};
#endif