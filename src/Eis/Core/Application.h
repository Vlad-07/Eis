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
		static Input& GetInput() { return s_Instance->m_Window->GetInput(); }
		// -----------


		void QueueTransition(uint32_t id);
		void QueueTransition(const std::string& name);
		void SetTransitionData(Buffer&& buf);
		void ClearTransitionData();

		void ShouldClose() { s_Instance->m_Running = false; }

	protected:
		template<typename T>
		void RegisterLayer(const std::string& name, const LayerLib::LayerFactory& factory = LayerLib::DefaultFactory<T>)
		{
			static_assert(std::is_base_of<Layer, T>(), "Registered layers must derive Eis::Layer!");
			m_LayerLib.RegisterLayer(factory, name);
		};
//		void RegisterOverlay();

		void SetEntryLayer(uint32_t id);
		void SetEntryLayer(const std::string& name);

	private:
		void Run();
		void RunLoop();

		// emscripten needs func ptr
		static void StaticRunLoop() { s_Instance->RunLoop(); }

		// Limited to a way lower value than is actually set due to oversleeping
		// Used only for limiting fps on focus lost
		void SetTargetFps(int fps) { s_Instance->m_TargetFrametime = Duration::FromHz((float)fps); }
		void WaitFPSLimit() const;

		void HandleTransition();

		void OnEvent(Event& event);
		bool OnWindowResize(WindowResizeEvent& e);
		bool OnWindowFocused(WindowFocusEvent& e);
		bool OnWindowLostFocus(WindowLostFocusEvent& e);
		bool OnWindowClose(WindowCloseEvent& e);

		friend int ::main(int argc, char** args);

	private:
		static Application* s_Instance;

		Scope<Window> m_Window;

		LayerLib m_LayerLib;
		LayerStack m_LayerStack;
		ImGuiLayer* m_ImGuiLayer; // Tehnically unsfe

		// Might not be the best thing to do
		struct ActiveLayerData
		{
			Layer* LayerPtr{}; // Tehnically unsafe
			std::string Name;
			int32_t Id{};
		} m_ActiveLayer;
		std::string m_QueuedLayerName{};
		int32_t m_QueuedLayerId = -1;
		std::optional<Buffer> m_QueuedTransitionData;

		bool m_Running = true;
		Duration m_TargetFrametime;
	};

	// Is defined in CLIENT
	Application* CreateApplication();
}