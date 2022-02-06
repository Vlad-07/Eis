#include <Eis.h>

class Sandbox : public Eis::Application
{
public:

	Sandbox()
	{
		// Push layers?
	}

	~Sandbox()
	{

	}
};

Eis::Application* Eis::CreateApplication()
{
	return new Sandbox();
}