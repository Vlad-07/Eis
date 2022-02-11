#pragma once

#include "Core.h"
#include "Window.h"
#include "LayerStack.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"

#include "ImGui/ImGuiLayer.h"

#include "Eis/Renderer/Objects/Shader.h"
#include "Eis/Renderer/Objects/Buffer.h"
#include "Eis/Renderer/Objects/VertexArray.h"

#include "Eis/Renderer/Objects/OrthographicCamera.h"

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

		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }
	private:
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		static Application* s_Instance;
		LayerStack m_LayerStack;

		bool m_Running = true;

	private:
		bool OnWindowClose(WindowCloseEvent event);

	private:
		std::shared_ptr<Shader> m_Shader;
		std::shared_ptr<VertexArray> m_VA;
		std::shared_ptr<VertexArray> m_SquareVA;

		OrthographicCamera m_Camera;
		float m_Rotation = 0.0f;
	};

	// Is defined in CLIENT:
	Application* CreateApplication();
}