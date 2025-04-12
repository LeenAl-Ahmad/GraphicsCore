#ifndef SPRITESHEET_H
#define SPRITESHEET_H

#include "Texture.h"
#include "SpriteAnim.h"
#include "BasicStructs.h"

enum AnimationNames
{
    EN_AN_IDLE = 0,
    EN_AN_RUN,
    EN_AN_TWO_COMBO_ATTACK,
    EN_AN_DEATH,
    EN_AN_HURT,
    EN_AN_JUMP_UP_FALL,
    EN_AN_EDGE_GRAB,
    EN_AN_EDGE_IDLE,
    EN_AN_WALL_SIDE,
    EN_AN_CROUCH,
    EN_AN_DASH,
    EN_AN_DASH_ATTACH,
    EN_AN_SLIDE,
    EN_AN_LADDER_GRAB,
    EN_AN_SMOKE_RISE, 
    EN_AN_ROLL, 
    EN_AN_ROCK,
    EN_AN_PAPER,
    EN_AN_SCISSOR
};

class SpriteSheet : public Texture
{
public:
    // Constructors/Destructors
    SpriteSheet();
    ~SpriteSheet();

    // Methods
    void Serialize(std::ostream& _stream) override;
    void Deserialize(std::istream& _stream) override;
    void ToString() override;

    void SetSize(byte _rows, byte _columns, short _clipSizeX, short _clipSizeY);
    void AddAnimation(AnimationNames _name, short _clipStart, short _clipCount, float _clipSpeed);
    void Update(AnimationNames _name, float _deltaTime, Rect* _r);
    Rect Update(AnimationNames _name, float _deltaTime);
    int GetCurrentClip(AnimationNames _name);
    void ResetCurrentClip(AnimationNames _n);

    // Members
    static ObjectPool<SpriteSheet>* Pool;

private:
    // Members
    byte m_rows;
    byte m_columns;
    short m_clipSizeX;
    short m_clipSizeY;
    map<AnimationNames, SpriteAnim*> m_animations;
};

#endif // SPRITESHEET_H
