#include "Eispch.h"
#include "Application.h"

#include <GLFW/glfw3.h> // must NOT be here

#include <imgui.h>

#include "Eis/Core/Random.h"
#include "Eis/Input/Input.h"
#include "Eis/Rendering/Renderer/Renderer2D.h"


namespace Eis
{
	Application* Application::s_Instance = nullptr;

	Application::Application(WindowProps props) : m_Running(true), m_LastFrameTime(0.0f)
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

		#ifndef EIS_PLATFORM_WEB
		while (m_Running)
		{
			RunLoop();
		}
		#else
		emscripten_set_main_loop(RunLoop, 0, true);
		#endif
	}

	void Application::RunLoop()
	{
		EIS_PROFILE_FUNCTION();

		float time = (float)glfwGetTime(); // TODO: frametime should be platform specific
		const TimeStep timeStep = time - s_Instance->m_LastFrameTime;
		s_Instance->m_LastFrameTime = time;

		// TODO: FixedUpdate

		s_Instance->m_Window->PollEvents();
		// TODO: fps limiter
		// TODO: limit fps on focus lost
		{
			EIS_PROFILE_SCOPE("LayerStack Update");

			for (Layer* layer : s_Instance->m_LayerStack)
				layer->Update(timeStep);
		}

		if (!s_Instance->m_Window->IsIconified())
		{
			{
				EIS_PROFILE_SCOPE("LayerStack Render");

				for (Layer* layer : s_Instance->m_LayerStack)
					layer->Render();
			}

			s_Instance->m_ImGuiLayer->Begin();
			{
				EIS_PROFILE_SCOPE("LayerStack ImGuiRender");

				for (Layer* layer : s_Instance->m_LayerStack)
					layer->ImGuiRender();
			}
			s_Instance->m_ImGuiLayer->End();
		}

		s_Instance->m_Window->SwapBuffers();
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
			if (e.Handled)
				break;
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		EIS_PROFILE_FUNCTION();

		m_LayerStack.PushLayer(layer);
		layer->Attach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		EIS_PROFILE_FUNCTION();

		m_LayerStack.PushOverlay(overlay);
		overlay->Attach();
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		EIS_PROFILE_FUNCTION();

		Renderer2D::OnWindowResized(e.GetSize().x, e.GetSize().y);

		return false;
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}
}