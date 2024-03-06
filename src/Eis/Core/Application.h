#pragma once

#include "Eis/Core/Core.h"
#include "Eis/Core/Window.h"
#include "Eis/Core/LayerStack.h"
#include "Eis/Core/TimeStep.h"

#include "Eis/Events/Event.h"
#include "Eis/Events/ApplicationEvent.h"

#include "Eis/ImGui/ImGuiLayer.h"

int main(int argc, char** args);

namespace Eis
{
	class Application
	{
	public:
		Application(WindowProps props = WindowProps());
		virtual ~Application();


		void OnEvent(Event& event);		
		
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		static Application& Get() { return *s_Instance; }
		Window& GetWindow() { return *m_Window; }

		static void ShouldClose() { s_Instance->m_Running = false; }

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

		bool m_Running;
		bool m_Minimized;
		float m_LastFrameTime;
	};

	// Is defined in CLIENT
	Application* CreateApplication();
}