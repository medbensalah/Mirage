#pragma once

#include <Mirage.h>

struct ParticleProps
{
    Vec2 Position;
    Vec2 Velocity, VelocityVariation;
    Vec4 ColorBegin, ColorEnd;
    float SizeBegin, SizeEnd, SizeVariation;
    float LifeTime = 1.0f;
};

class ParticleSystem
{
public:
    ParticleSystem();

    void OnUpdate(float ts);
    void OnRender(Mirage::OrthographicCamera& camera);

    void Emit(const ParticleProps& particleProps);
private:
    struct Particle
    {
        Vec2 Position;
        Vec2 Velocity;
        Vec4 ColorBegin, ColorEnd;
        float Rotation = 0.0f;
        float SizeBegin, SizeEnd;

        float LifeTime = 1.0f;
        float LifeRemaining = 0.0f;

        bool Active = false;
    };
    std::vector<Particle> m_ParticlePool;
    uint32_t m_PoolIndex = 999;
};