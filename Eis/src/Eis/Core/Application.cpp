#include "Eispch.h"
#include "Application.h"

#include "imgui.h"

#include "../Input/Input.h"

#include "Eis/Renderer/Renderer/Renderer.h"
#include "Eis/Core/Random.h"

#include <GLFW/glfw3.h>

namespace Eis
{
	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		EIS_PROFILE_FUNCTION();

		EIS_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = Window::Create();
		m_Window->SetEventCallback(EIS_BIND_EVENT_FN(Application::OnEvent));
		m_Window->SetVSync(true);

		Renderer::Init();
		Random::Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}

	Application::~Application()
	{
		Renderer::Shutdown();
	}

	void Application::Run()
	{
		EIS_PROFILE_FUNCTION();

		while (m_Running)
		{
			EIS_PROFILE_SCOPE("RunLoop frame");

			float time = (float)glfwGetTime(); // TODO: frametime should be in platform specific
			TimeStep timeStep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			if (!m_Minimized)
			{
				{
					EIS_PROFILE_SCOPE("LayerStack OnUpdate");

					for (Layer* layer : m_LayerStack)
						layer->OnUpdate(timeStep);
				}
			
				m_ImGuiLayer->Begin();
				{
					EIS_PROFILE_SCOPE("LayerStack OnImGuiRender");

					for (Layer* layer : m_LayerStack)
						layer->OnImGuiRender();
				}
				m_ImGuiLayer->End();
			}

			m_Window->OnUpdate();
		}
	}

	void Application::OnEvent(Event& e)
	{
		EIS_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(EIS_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(EIS_BIND_EVENT_FN(Application::OnWindowResize));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			(*--it)->OnEvent(e);
			if (e.m_Handled)
				break;
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		EIS_PROFILE_FUNCTION();

		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		EIS_PROFILE_FUNCTION();

		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	bool Application::OnWindowResize(WindowResizeEvent e)
	{
		EIS_PROFILE_FUNCTION();

		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}

		m_Minimized = false;

		Renderer::OnWindowResized(e.GetWidth(), e.GetHeight());
		
		return false;
	}

	bool Application::OnWindowClose(WindowCloseEvent e)
	{
		m_Running = false;
		return true;
	}
}