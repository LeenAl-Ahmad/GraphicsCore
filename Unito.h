#ifndef UNIT_H
#define UNIT_H

#include "SpriteSheet.h"
#include "SoundEffect.h"
#include "Timing.h"
#include "AudioController.h"
#include "InputController.h"

class Unit : public Texture
{
public:
    Unit();
    ~Unit();

    // Resource overrides
    
    void AssignNonDefaultValues() override;
    virtual Rect Update(AnimationNames _name, float _deltaTime);

   
    SpriteSheet* GetWarrior() { return warrior; }

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

    SpriteSheet* warrior;
};
#endif