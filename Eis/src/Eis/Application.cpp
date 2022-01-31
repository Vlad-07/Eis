#include "Eispch.h"
#include "Application.h"

namespace Eis
{
#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

	Application::Application()
	{
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{

		m_Window->OnUpdate();

		while (m_Running)
		{
			m_Window->OnUpdate();
		}
	}
	
	void Application::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));

		EIS_CORE_TRACE("{0}", event);
	}

	bool Application::OnWindowClose(WindowCloseEvent event)
	{
		m_Running = false;
		return true;
	}
}