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

		// If no entry set, default to first registered layer
		if (m_LayerStack.GetSize() <= 1)
			SetEntryLayer(0);

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

		HandleTransition();

		WaitFPSLimit();
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


	void Application::SetEntryLayer(uint32_t id)
	{
		if (m_LayerStack.GetSize() > 1)
		{
			EIS_ERROR("Entry layer already set!");
			return;
		}

		auto layer = m_LayerLib.MakeLayer(id, m_QueuedTransitionData);
		ClearTransitionData();

		m_ActiveLayer.LayerPtr = layer.get();
		m_ActiveLayer.Id = id;
		m_ActiveLayer.Name = layer->GetName();

		m_LayerStack.PushLayer(std::move(layer));
	}

	void Application::SetEntryLayer(const std::string& name)
	{
		if (m_LayerStack.GetSize() > 1)
		{
			EIS_ERROR("Entry layer already set!");
			return;
		}

		auto layer = m_LayerLib.MakeLayer(name, m_QueuedTransitionData);
		ClearTransitionData();

		m_ActiveLayer.LayerPtr = layer.get();
		m_ActiveLayer.Id = m_LayerLib.GetLayerId(name);
		m_ActiveLayer.Name = name;

		m_LayerStack.PushLayer(std::move(layer));
	}


	void Application::QueueTransition(uint32_t id)
	{
		if (s_Instance->m_ActiveLayer.Id == id)
		{
			EIS_CORE_WARN("Request to transition to active layer ignored!");
			return;
		}

		s_Instance->m_QueuedLayerId = id;
		s_Instance->m_QueuedLayerName.clear();
	}

	void Application::QueueTransition(const std::string& name)
	{
		if (s_Instance->m_ActiveLayer.Name == name)
		{
			EIS_CORE_WARN("Request to transition to active layer ignored!");
			return;
		}

		s_Instance->m_QueuedLayerName = name;
		s_Instance->m_QueuedLayerId = -1;
	}

	void Application::SetTransitionData(Buffer&& buf)
	{
		m_QueuedTransitionData.emplace(std::move(buf));
	}

	void Application::ClearTransitionData()
	{
		m_QueuedTransitionData.reset();
	}


	void Application::HandleTransition()
	{
		// this is needlessly complicated

		if (m_QueuedLayerId != -1)
		{
			m_LayerStack.PopLayer(m_ActiveLayer.LayerPtr);

			auto layer = m_LayerLib.MakeLayer(m_QueuedLayerId, m_QueuedTransitionData);
			ClearTransitionData();

			m_ActiveLayer.LayerPtr = layer.get();
			m_ActiveLayer.Id = m_QueuedLayerId;
			m_ActiveLayer.Name = m_ActiveLayer.LayerPtr->GetName();

			m_LayerStack.PushLayer(std::move(layer));

			m_QueuedLayerId = -1;

//			EIS_CORE_INFO("Transitioned to {}.", m_ActiveLayer.Name);
		}
		else if (!m_QueuedLayerName.empty())
		{
			m_LayerStack.PopLayer(m_ActiveLayer.LayerPtr);

			auto layer = m_LayerLib.MakeLayer(m_QueuedLayerName, m_QueuedTransitionData);
			ClearTransitionData();

			m_ActiveLayer.LayerPtr = layer.get();
			m_ActiveLayer.Id = m_LayerLib.GetLayerId(m_QueuedLayerName);
			m_ActiveLayer.Name = m_QueuedLayerName;

			m_LayerStack.PushLayer(std::move(layer));

			m_QueuedLayerName.clear();

//			EIS_INFO("Transitioned to {}.", m_ActiveLayer.Name);
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

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		EIS_PROFILE_FUNCTION();

		Renderer2D::OnWindowResized(e.GetSize().x, e.GetSize().y);
		ExpRenderer::OnWindowResized(e.GetSize().x, e.GetSize().y);

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