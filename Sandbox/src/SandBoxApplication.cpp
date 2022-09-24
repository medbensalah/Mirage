#include "Sandbox2D.h"
#include "ExampleLayer.h"


/* *******Entry point******* */
#include "Mirage/Core/EntryPoint.h"

class Sandbox : public Mirage::Application
{
public:
    Sandbox()
    {
        // PushLayer(new ExampleLayer());
        PushLayer(new Sandbox2D());
    }

    ~Sandbox()
    {
    }
};

Mirage::Application* Mirage::CreateApplication()
{
    return new Sandbox();
}
