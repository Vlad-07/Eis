#include "Eispch.h"
#include "Application.h"

#include "Eis/Core/Core.h"
#include "Eis/Core/Random.h"
#include "Eis/Rendering/Renderer/Renderer2D.h"

#include "Eis/RenderingExp/ExpRenderer.h"

#include "Eis/ImGui/ImGuiLayer.h"


namespace Eis
{
	Application* Application::s_Instance = nullptr;


	Application::Application(WindowProps props)
	{
		EIS_PROFILE_FUNCTION();

		EIS_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		// Init sub-systems
		Time::Init();
		Random::Init();
		m_Window = Window::Create(props);
		m_Window->SetEventCallback(EIS_BIND_EVENT_FN(Application::OnEvent));
		Renderer2D::Init();

		// Init ImGui overlay
		Scope<Layer> imlayer = CreateScope<ImGuiLayer>();
		m_ImGuiLayer = static_cast<ImGuiLayer*>(imlayer.get());
		m_LayerStack.PushOverlay(std::move(imlayer));
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
		emscripten_set_main_loop(StaticRunLoop, 0, true);
		#endif
	}

	void Application::RunLoop()
	{
		EIS_PROFILE_FUNCTION();

		m_Window->PollEvents();

		Time::FrameStart();

		while (Time::ShouldRunFixedUpdate())
		{
			EIS_PROFILE_SCOPE("LayerStack FixedUpdate");

			for (Scope<Layer>& layer : m_LayerStack)
				layer->FixedUpdate();
		}

		{
			EIS_PROFILE_SCOPE("LayerStack Update");

			for (Scope<Layer>& layer : m_LayerStack)
				layer->Update();
		}

		if (!m_Window->IsIconified())
		{
			{
				EIS_PROFILE_SCOPE("LayerStack Render");

				for (Scope<Layer>& layer : m_LayerStack)
					layer->Render();
			}

			m_ImGuiLayer->Begin();
			{
				EIS_PROFILE_SCOPE("LayerStack ImGuiRender");

				for (Scope<Layer>& layer : m_LayerStack)
					layer->ImGuiRender();
			}
			m_ImGuiLayer->End();
		}

		m_Window->SwapBuffers();
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


	void Application::OnEvent(Event& e)
	{
		EIS_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowResizeEvent>(EIS_BIND_EVENT_FN(Application::OnWindowResize));
		dispatcher.Dispatch<WindowCloseEvent>(EIS_BIND_EVENT_FN(Application::OnWindowClose));

		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
		{
			(*it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		Renderer2D::OnWindowResized(e.GetSize().x, e.GetSize().y);
		//ExpRenderer::OnWindowResized(e.GetSize().x, e.GetSize().y);

		return false;
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}
}