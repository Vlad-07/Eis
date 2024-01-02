#pragma once

#include "Eis/Core/Core.h"

#include "Eis/Core/TimeStep.h"
#include "Eis/Core/Window.h"
#include "Eis/Core/LayerStack.h"

#include "Eis/Events/Event.h"
#include "Eis/Events/ApplicationEvent.h"

#include "Eis/ImGui/ImGuiLayer.h"

int main(int argc, char** args);

namespace Eis
{
	class Application
	{
	public:
		Application();
		virtual ~Application();


		void OnEvent(Event& event);		
		
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		static Application& Get() { return *s_Instance; }
		Window& GetWindow() { return *m_Window; }

	private:
		void Run();

		bool OnWindowResize(WindowResizeEvent e);
		bool OnWindowClose(WindowCloseEvent e);

		friend int ::main(int argc, char** args);

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