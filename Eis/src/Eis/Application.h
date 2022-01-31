#pragma once

#include "Core.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"
#include "Window.h"

namespace Eis
{
	class EIS_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& event);
	private:
		bool OnWindowClose(WindowCloseEvent event);

		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
	};

	// Is defined in CLIENT:
	Application* CreateApplication();
}