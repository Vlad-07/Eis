#pragma once

#include "Eis/Core/Window.h"
#include "Eis/Core/LayerStack.h"
#include "Eis/Core/Time.h"

#include "Eis/Events/Event.h"
#include "Eis/Events/ApplicationEvent.h"

#include <filesystem>


int main(int argc, char** args);

namespace Eis
{
	struct ApplicationCommandLineArgs
	{
		int Count = 0;
		char** Args = nullptr;

		const char* operator[](int index) const
		{
			EIS_CORE_ASSERT(index < Count);
			return Args[index];
		}
	};

	struct ApplicationSpecification
	{
		std::string Name = "App";
		std::filesystem::path WorkingDirectory;
		ApplicationCommandLineArgs CommandLineArgs;
	};


	class ImGuiLayer;

	class Application
	{
	public:
		Application(const ApplicationSpecification& spec);
		virtual ~Application();

		static Application& Get() { return *s_Instance; }
		static Window& GetWindow() { return *s_Instance->m_Window; }

		void Close() { s_Instance->m_Running = false; }

		const ApplicationSpecification& GetSpec() const { return m_Spec; }

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

	private:
		ApplicationSpecification m_Spec;

		Scope<Window> m_Window;

		LayerStack m_LayerStack;
		ImGuiLayer* m_ImGuiLayer; // Tehnically unsafe

		bool m_Running{ true };

	private:
		static inline Application* s_Instance{};
		friend int ::main(int argc, char** args);
	};

	// Is defined in CLIENT
	Application* CreateApplication(ApplicationCommandLineArgs args);
}