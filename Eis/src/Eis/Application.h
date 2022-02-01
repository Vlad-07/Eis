#pragma once

#include "Core.h"
#include "Window.h"
#include "LayerStack.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"

namespace Eis
{
	class EIS_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& event);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
	private:
		std::unique_ptr<Window> m_Window;
		LayerStack m_LayerStack;

		bool m_Running = true;

		bool OnWindowClose(WindowCloseEvent event);
	};

	// Is defined in CLIENT:
	Application* CreateApplication();
}