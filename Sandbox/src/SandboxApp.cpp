#include <Eis.h>

class Sandbox : public Eis::Application
{
public:

	Sandbox()
	{
		PushOverlay(new Eis::ImGuiLayer());
	}

	~Sandbox()
	{

	}
};

Eis::Application* Eis::CreateApplication()
{
	return new Sandbox();
}