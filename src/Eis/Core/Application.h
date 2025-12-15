#pragma once

#include "Eis/Core/Window.h"
#include "Eis/Core/LayerStack.h"

#include "Eis/Core/Time.h"
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
		static Window& GetWindow() { return *s_Instance->m_Window; }

		static void ShouldClose() { s_Instance->m_Running = false; }

	private:
		void Run();
		static void RunLoop(); // HACK: ugly s_Instance-> everywhere but emscripten needs func ptr

		// Limited to a way lower value than is actually set due to oversleeping
		// Used only for limiting fps on focus lost
		static void SetTargetFps(int fps) { s_Instance->m_TargetFrametime = Duration::FromHz((float)fps); }
		void WaitFPSLimit() const;

		bool OnWindowResize(WindowResizeEvent& e);
		bool OnWindowFocused(WindowFocusedEvent& e);
		bool OnWindowLostFocus(WindowLostFocusEvent& e);
		bool OnWindowClose(WindowCloseEvent& e);

		friend int ::main(int argc, char** args);

	private:
		static Application* s_Instance;

		Scope<Window> m_Window;

		LayerStack m_LayerStack;
		ImGuiLayer* m_ImGuiLayer;

		bool m_Running = true;
		Duration m_TargetFrametime;
	};

	// Is defined in CLIENT
	Application* CreateApplication();
}