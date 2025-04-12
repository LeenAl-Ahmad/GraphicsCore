#ifndef PLAYER_STATE_H
#define PLAYER_STATE_H

#include "StandardIncludes.h"

class Player;
class RockState;
class PaperState;
class ScissorState;
class NormState;
class RollState;

class PlayerState
{
public:
    // Constructors / Destructors
    PlayerState() {}
    virtual ~PlayerState() {}

    // Accessors
    static RollState* GetRollState() { return &m_roll; }
    static NormState* GetNormState() { return &m_norm; }
    static RockState* GetRockState() { return &m_rock; }
    static PaperState* GetPaperState() { return &m_paper; }
    static ScissorState* GetScissorState() { return &m_scissor; }
    
    // Methods
    virtual void HandleInput(Player* _heroine, SDL_Event _event) = 0;
    virtual void Update(Player* _heroine, float _deltaTime) = 0;

private:
    // Members
    static NormState m_norm;
    static RollState m_roll;
    static RockState m_rock;
    static PaperState m_paper;
    static ScissorState m_scissor;
};

class NormState :public PlayerState
{
public:
    NormState(){}
    virtual ~NormState(){}

    void HandleInput(Player* player, SDL_Event m_event) override;
    void Update(Player* player, float _deltaTime)override;
};

class RollState :public PlayerState
{
public:
    RollState() {}
    virtual ~RollState() {}

    void HandleInput(Player* player, SDL_Event m_event) override;
    void Update(Player* player, float _deltaTime)override;
};

class RockState :public PlayerState
{
public:
    RockState(){}
    virtual ~RockState(){}

    void HandleInput(Player* player, SDL_Event m_event) override;
    void Update(Player* player, float _deltaTime)override;
};

class PaperState : public PlayerState
{
public:
    PaperState() {}
    virtual ~PaperState() {}

    void HandleInput(Player* _heroine, SDL_Event _event) override;
    void Update(Player* _heroine, float _deltaTime) override;
};

class ScissorState : public PlayerState
{
public:
    ScissorState(){}
    virtual ~ScissorState() {}

    void HandleInput(Player* _heroine, SDL_Event _event) override;
    void Update(Player* _heroine, float _deltaTime) override;
};
#endif // PLAYER_STATE_H


