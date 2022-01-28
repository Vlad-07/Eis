#include <Eis.h>

class Sandbox : public Eis::Application
{
public:

	Sandbox()
	{

	}

	~Sandbox()
	{

	}
};

Eis::Application* Eis::CreateApplication()
{
	return new Sandbox();
}