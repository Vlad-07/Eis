#pragma once

#include "Eis/Core/Core.h"

#include "Eis/Core/TimeStep.h"
#include "Eis/Core/Window.h"
#include "Eis/Core/LayerStack.h"

#include "Eis/Events/Event.h"
#include "Eis/Events/ApplicationEvent.h"

#include "Eis/ImGui/ImGuiLayer.h"

namespace Eis
{
	class Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& event);		
		
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }

	private:
		bool OnWindowResize(WindowResizeEvent e);
		bool OnWindowClose(WindowCloseEvent e);

	private:
		static Application* s_Instance;
		Eis::Scope<Window> m_Window;

		LayerStack m_LayerStack;
		ImGuiLayer* m_ImGuiLayer;

		float m_LastFrameTime;

		bool m_Running;
		bool m_Minimized;
	};

	// Is defined in CLIENT
	Application* CreateApplication();
}