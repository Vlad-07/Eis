#include "Eispch.h"
#include "Application.h"

#include <imgui.h>

#include "Eis/Core/Core.h"
#include "Eis/Core/Random.h"
#include "Eis/Input/Input.h"
#include "Eis/Rendering/Renderer/Renderer2D.h"


namespace Eis
{
	Application* Application::s_Instance = nullptr;

	Application::Application(WindowProps props)
	{
		EIS_PROFILE_FUNCTION();

		EIS_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		Time::Init();
		Random::Init();

		m_Window = Window::Create(props);
		m_Window->SetEventCallback(EIS_BIND_EVENT_FN(Application::OnEvent));

		Renderer2D::Init();

		Scope<ImGuiLayer> imlayer = CreateScope<ImGuiLayer>();
		m_ImGuiLayer = imlayer.get();
		PushOverlay(std::move(imlayer));
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

		s_Instance->m_Window->PollEvents();

		Time::FrameStart();

		while (Time::ShouldRunFixedUpdate())
		{
			EIS_PROFILE_SCOPE("LayerStack FixedUpdate");

			for (Scope<Layer>& layer : s_Instance->m_LayerStack)
				layer->FixedUpdate();
		}

		{
			EIS_PROFILE_SCOPE("LayerStack Update");

			for (Scope<Layer>& layer : s_Instance->m_LayerStack)
				layer->Update();
		}

		if (!s_Instance->m_Window->IsIconified())
		{
			{
				EIS_PROFILE_SCOPE("LayerStack Render");

				for (Scope<Layer>& layer : s_Instance->m_LayerStack)
					layer->Render();
			}

			s_Instance->m_ImGuiLayer->Begin();
			{
				EIS_PROFILE_SCOPE("LayerStack ImGuiRender");

				for (Scope<Layer>& layer : s_Instance->m_LayerStack)
					layer->ImGuiRender();
			}
			s_Instance->m_ImGuiLayer->End();
		}

		s_Instance->m_Window->SwapBuffers();

		s_Instance->WaitFPSLimit();
	}

	void Application::WaitFPSLimit() const
	{
		// Prioritise vsync except in background
		if (m_Window->IsVSync() && m_Window->IsFocused())
			return;

		while (true)
		{
			Duration remaining = ChronoDuration(Time::Now() - Time::GetFrameStart());
			if (remaining >= m_TargetFrametime)
				break;

			// TODO: find a way to sleep more precise

			if (remaining > Duration::FromMs(2.0))
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
			else if (remaining > Duration::FromMs(1.0))
				std::this_thread::yield();
		}
	}

	void Application::OnEvent(Event& e)
	{
		EIS_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(EIS_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(EIS_BIND_EVENT_FN(Application::OnWindowResize));
		dispatcher.Dispatch<WindowFocusEvent>(EIS_BIND_EVENT_FN(Application::OnWindowFocused));
		dispatcher.Dispatch<WindowLostFocusEvent>(EIS_BIND_EVENT_FN(Application::OnWindowLostFocus));

		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
		{
			(*it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	void Application::PushLayer(Scope<Layer> layer)
	{
		EIS_PROFILE_FUNCTION();

		m_LayerStack.PushLayer(std::move(layer));
	}

	void Application::PushOverlay(Scope<Layer> overlay)
	{
		EIS_PROFILE_FUNCTION();

		m_LayerStack.PushOverlay(std::move(overlay));
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		EIS_PROFILE_FUNCTION();

		Renderer2D::OnWindowResized(e.GetSize().x, e.GetSize().y);

		return false;
	}

	bool Application::OnWindowFocused(WindowFocusEvent& e)
	{
		m_TargetFrametime = Duration::FromMs(0.0);
		return false;
	}

	bool Application::OnWindowLostFocus(WindowLostFocusEvent& e)
	{
		m_TargetFrametime = Duration::FromMs(30.0);
		return false;
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}
}