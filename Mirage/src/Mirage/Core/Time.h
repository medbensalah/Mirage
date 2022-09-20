#pragma once
namespace Mirage
{
    struct Time
    {
        friend class Application;

        float GetElapsedTimeInMilliseconds() { return m_ElapsedTime * 1000.0f; }
        float GetElapsedTimeInSeconds() { return m_ElapsedTime; }

        const float& ElapsedTime;
        const float& DeltaTime;
    private:
        Time(float time = 0.0f)
            : m_ElapsedTime(time), m_DeltaTime(0.0f), ElapsedTime(m_ElapsedTime), DeltaTime(m_DeltaTime)
        {
        }

        void Update(float time)
        {
            m_DeltaTime = time - m_ElapsedTime;
            m_ElapsedTime = time;
        }

        float m_ElapsedTime;
        float m_DeltaTime;
    };
}
