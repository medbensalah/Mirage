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
        MRG_INFO("Example layer :: Update");
    }

    void OnEvent(Mirage::Event& event) override
    {
        MRG_TRACE(event);
    }
};

class Sandbox : public Mirage::Application
{
public:
    Sandbox()
    {
        PushLayer(new ExampleLayer());
    }

    ~Sandbox()
    {
    }
};

Mirage::Application* Mirage::CreateApplication()
{
    return new Sandbox();
}
