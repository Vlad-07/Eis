#pragma once

#include "Eis/Core/Window.h"
#include "Eis/Core/LayerLib.h"
#include "Eis/Core/LayerStack.h"
#include "Eis/Core/Time.h"

#include "Eis/Events/Event.h"
#include "Eis/Events/ApplicationEvent.h"


int main(int argc, char** args);

namespace Eis
{
	class ImGuiLayer;

	class Application
	{
	public:
		Application(WindowProps props = WindowProps());
		virtual ~Application();


		// Engine only
		static Application& Get() { return *s_Instance; }
		static Window& GetWindow() { return *s_Instance->m_Window; }
		// -----------


		void Close() { s_Instance->m_Running = false; }

	protected:
		void PushLayer(Scope<Layer> layer);
		void PushOverlay(Scope<Layer> overlay);

	private:
		void Run();
		void RunLoop();

		// emscripten needs func ptr
		static void StaticRunLoop() { s_Instance->RunLoop(); }

		void OnEvent(Event& event);
		bool OnWindowResize(WindowResizeEvent& e);
		bool OnWindowClose(WindowCloseEvent& e);

		friend int ::main(int argc, char** args);

	private:
		static Application* s_Instance;

		Scope<Window> m_Window;

		LayerStack m_LayerStack;
		ImGuiLayer* m_ImGuiLayer; // Tehnically unsafe

		bool m_Running{ true };
	};

	// Is defined in CLIENT
	Application* CreateApplication();
}