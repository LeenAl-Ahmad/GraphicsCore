#include "SpriteAnim.h"

ObjectPool<SpriteAnim>* SpriteAnim::Pool;

SpriteAnim::SpriteAnim()
{
    ClearMemory();
}

SpriteAnim::~SpriteAnim()
{
}

void SpriteAnim::Create(short _clipStart, short _clipCount, float _clipSpeed)
{
    m_clipStart = _clipStart;
    m_clipCount = _clipCount;
    m_clipSpeed = _clipSpeed;
    m_clipCurrent = _clipStart;
    m_clipEnd = m_clipStart + m_clipCount;
    m_elapsedTime = 0.0f;
}

void SpriteAnim::ClearMemory()
{
    m_clipStart = 0;
    m_clipCount = 0;
    m_clipCurrent = 0;
}

void SpriteAnim::Update(float _d)
{
    m_elapsedTime += _d;

    // Calculate the current frame based on elapsed time and animation speed
    float frameTime = 1.0f / m_clipSpeed; // Time per frame
    int frameIndex = static_cast<int>(m_elapsedTime / frameTime) % m_clipCount;

    // Set the current frame
    m_clipCurrent = m_clipStart + frameIndex;

    if (m_clipCurrent >= m_clipEnd)
    {
        m_clipCurrent = m_clipStart;
    }
}

void SpriteAnim::Serialize(std::ostream& _stream)
{
    _stream.write(reinterpret_cast<char*>(&m_clipStart), sizeof(m_clipStart));
    _stream.write(reinterpret_cast<char*>(&m_clipCount), sizeof(m_clipCount));
    _stream.write(reinterpret_cast<char*>(&m_clipSpeed), sizeof(m_clipSpeed));
    Resource::Serialize(_stream);
}

void SpriteAnim::Deserialize(std::istream& _stream)
{
    _stream.read(reinterpret_cast<char*>(&m_clipStart), sizeof(m_clipStart));
    _stream.read(reinterpret_cast<char*>(&m_clipCount), sizeof(m_clipCount));
    _stream.read(reinterpret_cast<char*>(&m_clipSpeed), sizeof(m_clipSpeed));
    m_clipEnd = m_clipStart + m_clipCount;
    Resource::Deserialize(_stream);
}

void SpriteAnim::ToString()
{
    cout << "SPRITE ANIMATION: ";
    cout << " Clip Start: " << m_clipStart;
    cout << " Clip Count: " << m_clipCount << endl;
    Resource::ToString();
}
