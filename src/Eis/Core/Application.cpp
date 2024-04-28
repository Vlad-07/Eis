#include "Eispch.h"
#include "Application.h"

#include "Eis/Core/Random.h"
#include "Eis/Input/Input.h"
#include "Eis/Rendering/Renderer/Renderer2D.h"

#include <GLFW/glfw3.h>
#include "imgui.h"


namespace Eis
{
	Application* Application::s_Instance = nullptr;

	Application::Application(WindowProps props) : m_Running(true), m_Minimized(false), m_LastFrameTime(0.0f)
	{
		EIS_PROFILE_FUNCTION();

		EIS_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = Window::Create(props);
		m_Window->SetEventCallback(EIS_BIND_EVENT_FN(Application::OnEvent));

		Renderer2D::Init();
		Random::Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}

	Application::~Application()
	{
		EIS_PROFILE_FUNCTION();

		Renderer2D::Shutdown();
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
			// TODO: fps limiter
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

		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
		{
			(*it)->OnEvent(e);
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

		Renderer2D::OnWindowResized(e.GetWidth(), e.GetHeight());
		
		return false;
	}

	bool Application::OnWindowClose(WindowCloseEvent e)
	{
		m_Running = false;
		return true;
	}
}