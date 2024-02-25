#pragma once
class AsteroidManager
{
public:
    static AsteroidManager& getInstance();

    AsteroidManager(const AsteroidManager&)    = delete;
    AsteroidManager(AsteroidManager&&)         = delete;
    AsteroidManager& operator=(const AsteroidManager&) = delete;
    AsteroidManager& operator=(AsteroidManager&&)      = delete;

    void init();
    void update(float deltaTime);
    void deleteAsteroid(const std::string& id);

private:
    AsteroidManager();
    float m_totalRunTime   = 0.0f;
    float m_timeAccumulator   = 0.0f;
    float m_baseThreshold   = 5.f;
    float m_dynamicValue      = 0.0f;
    int  m_asteroidCounter;
    void makeAsteroid();
};
