#include <Eis.h>
#include <Eis/Core/EntryPoint.h>

#include <imgui.h> // Should remove from sandbox aditional include dirs

#include "DemoLayer.h"


class Sandbox : public Eis::Application
{
public:
	Sandbox()
	{
		PushLayer(new DemoLayer());
	}

	~Sandbox() = default;
};

Eis::Application* Eis::CreateApplication()
{
	return new Sandbox();
}