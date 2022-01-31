#include "Eispch.h"
#include "Application.h"

namespace Eis
{

	Application::Application()
	{
		m_Window = std::unique_ptr<Window>(Window::Create());
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{

		m_Window->OnUpdate();

		m_Window->~Window();
	}
}