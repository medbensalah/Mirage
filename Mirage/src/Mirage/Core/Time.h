#pragma once
namespace Mirage
{
    struct Time
    {
        friend class Application;
    public:
        float GetElapsedTimeInMilliseconds() { return m_ElapsedTime * 1000.0f; }
        float GetElapsedTimeInSeconds() { return m_ElapsedTime; }

        const float& ElapsedTime;
        const float& DeltaTime;
    private:
        Time(float time = 0.0f)
            : ElapsedTime(m_ElapsedTime), DeltaTime(m_DeltaTime), m_ElapsedTime(time), m_DeltaTime(0.0f)
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

    struct Timer
    {
    public:
        Timer()
        {
            Reset();
        }

        void Timer::Reset()
        {
            m_Start = std::chrono::high_resolution_clock::now();
        }

        float Timer::Elapsed()
        {
            return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - m_Start).count() * 0.001f * 0.001f * 0.001f;
        }

        float Timer::ElapsedMillis()
        {
            return Elapsed() * 1000.0f;
        }

    private:
        std::chrono::time_point<std::chrono::high_resolution_clock> m_Start;
    };

}
