#include "Sandbox2D.h"
#include "ExampleLayer.h"


/* *******Entry point******* */
#include "Mirage/Core/EntryPoint.h"

class Sandbox : public Mirage::Application
{
public:
    Sandbox(const Mirage::ApplicationSpecification& specification)
	    : Mirage::Application(specification)
    {
        // PushLayer(new ExampleLayer());
        PushLayer(new Sandbox2D());
    }

    ~Sandbox()
    {
    }
};

Mirage::Application* Mirage::CreateApplication(Mirage::ApplicationCommandLineArgs args)
{
	ApplicationSpecification specification;
	
	specification.Name = "Sandbox";
	specification.WorkingDirectory = "../Mirage Editor";
	specification.CommandLineArgs = args;
	
	return new Sandbox(specification);
}
