#include "Sandbox2D.h"
#include <ImGui/imgui.h>
#include <glm/gtc/type_ptr.inl>

template <typename Fn>
class Timer
{
public:
    Timer(const char* name, Fn&& fn)
        : m_Name(name), m_Stopped(false), m_Function(fn)
    {
        m_StartTimepoint = std::chrono::high_resolution_clock::now();
    }

    ~Timer()
    {
        if (!m_Stopped)
            Stop();
    }

    void Stop()
    {
        auto endTimepoint = std::chrono::high_resolution_clock::now();

        long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().
            count();
        long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().
            count();

        float duration = (end - start) * 0.001f;
        //  m_Durations.push_back(duration);
        m_Function({m_Name, duration});

        m_Stopped = true;
    }

private:
    const char* m_Name;
    bool m_Stopped;
    std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimepoint;
    std::vector<uint32_t> m_Durations;

    Fn m_Function;
};

#define PROFILE_SCOPE(name) Timer timer##__LINE__(name, [&](ProfileResult profileResult) { m_ProfileResults.push_back(profileResult); })

Sandbox2D::Sandbox2D()
    : Layer("SandBox2D"), m_CameraController(1280.0f / 720.0f, true)
{
    m_texture = Mirage::Texture2D::Create("assets/textures/CheckerBoard.png");
}

void Sandbox2D::OnAttach()
{
}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(float DeltaTime)
{
    // Update
    PROFILE_SCOPE("Sandbox2D::OnUpdate");
    m_DeltaTime = DeltaTime;
    m_CameraController.OnUpdate(DeltaTime);


    // Render
    Mirage::RenderCommand::SetClearColor({0.15f, 0.15f, 0.15f, 1.0f});
    Mirage::RenderCommand::Clear();
    
    Mirage::Renderer2D::BeginScene(m_CameraController.GetCamera());

    Mirage::Renderer2D::Draw::Quad(quad);

    Mirage::Renderer2D::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
    ImGui::Begin("Settings");
    ImGui::DragFloat3("Position", glm::value_ptr(m_Position), 0.05f);
    ImGui::DragFloat3("Rotation", glm::value_ptr(m_Rotation), 0.05f, -180.0f, 180.0f);
    ImGui::DragFloat3("Scale", glm::value_ptr(m_Scale), 0.05f);

    ImGui::Spacing();
    ImGui::ColorEdit4("Square Color", glm::value_ptr(m_Color));

    ImGui::Spacing();
    ImGui::DragFloat2("Tiling", glm::value_ptr(m_Tiling), 0.05f);
    ImGui::DragFloat2("Offset", glm::value_ptr(m_Offset), 0.05f);

    ImGui::Spacing();
    ImGui::Separator();

    for (auto& result : m_ProfileResults)
    {
        char label[50];
        sprintf_s(label, 50, "%.3fms : %s.", result.Time, result.Name);
        ImGui::Text(label);
    }
    m_ProfileResults.clear();

    ImGui::End();
}

void Sandbox2D::OnEvent(Mirage::Event& e)
{
    Layer::OnEvent(e);

    m_CameraController.OnEvent(e);
}
