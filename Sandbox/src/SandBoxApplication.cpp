#include <Mirage.h>

class Sandbox : public Mirage::Application {
public:
	Sandbox()
	{

	}

	~Sandbox()
	{

	}
};

Mirage::Application* Mirage::CreateApplication()
{
	return new Sandbox();
}