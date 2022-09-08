#include <Mirage.h>

class ExampleLayer : public Mirage::Layer
{
public:
    ExampleLayer()
        : Layer("Example")
    {
    }

    void OnUpdate() override
    {
        // if(Mirage::Input::IsKeyPressed(Mirage::MRG_Key_Tab))
        //     MRG_INFO("Tab key is pressed (poll)!");
    }

    void OnEvent(Mirage::Event& event) override
    {
        // if(event.GetEventType() == Mirage::EventType::KeyTyped)
        // {
        //     Mirage::KeyTypedEvent& e = (Mirage::KeyTypedEvent&)event;
        //     MRG_TRACE("{0}", (char)e.GetKeyCode());
        // }
    }
};

class Sandbox : public Mirage::Application
{
public:
    Sandbox()
    {
        PushLayer(new ExampleLayer());
        PushOverlay(new Mirage::ImGuiLayer());
    }

    ~Sandbox()
    {
    }
};

Mirage::Application* Mirage::CreateApplication()
{
    return new Sandbox();
}
