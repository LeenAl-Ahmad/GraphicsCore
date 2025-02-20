#ifndef SPRITEANIM_H
#define SPRITEANIM_H

#include "Resource.h"

class SpriteAnim : Resource
{
public:
    // Constructors/Destructors
    SpriteAnim();
    virtual ~SpriteAnim();

    // Accessors
    short GetClipCurrent() { return (short)m_clipCurrent; }

    // Methods
    virtual void Serialize(std::ostream& _stream);
    virtual void Deserialize(std::istream& _stream);
    virtual void ToString();

    void ClearMemory();
    void Create(short _clipStart, short _clipCount, float _clipSpeed);
    void Update(float _d);

    // Members
    static ObjectPool<SpriteAnim>* Pool;

private:
    // Members
    short m_clipStart;
    short m_clipCount;
    short m_clipEnd;
    short m_clipCurrent;
    float m_clipSpeed;
    float m_elapsedTime;
};

#endif // SPRITEANIM_H
