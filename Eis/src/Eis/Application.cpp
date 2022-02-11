#include "Eispch.h"
#include "Application.h"

#include "imgui.h"

#include "Input.h"

#include "Eis/Renderer/Renderer/Renderer.h"

namespace Eis
{
#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application()
		: m_Camera(-1.6f, 1.6f, -0.9f, 0.9f)
	{
		EIS_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);


		m_VA.reset(VertexArray::Create());

		float verts[3 * 7] = {
			-0.5f, -0.5f, 0.0f,   0.9f, 0.1f, 0.1f, 0.9f,
			 0.5f, -0.5f, 0.0f,   0.1f, 0.9f, 0.1f, 0.9f,
			 0.0f,  0.5f, 0.0f,   0.1f, 0.1f, 0.9f, 0.9f
		};
		std::shared_ptr<VertexBuffer> VB;
		VB.reset(VertexBuffer::Create(verts, sizeof(verts)));

		BufferLayout layout = {
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" }
		};
		VB->SetLayout(layout);
		m_VA->AddVertexBuffer(VB);

		uint32_t indices[3] = { 0, 1, 2 };
		std::shared_ptr<IndexBuffer> IB;
		IB.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VA->SetIndexBuffer(IB);


		float verts2[4 * 7] = {
			-0.75f, -0.75f, 0.0f,   0.1f, 0.1f, 0.8f, 1.0f,
			 0.75f, -0.75f, 0.0f,   0.1f, 0.9f, 0.1f, 1.0f,
			 0.75f,  0.75f, 0.0f,   0.1f, 0.1f, 0.9f, 1.0f,
			-0.75f,  0.75f, 0.0f,   0.9f, 0.1f, 0.1f, 1.0f
		};
		m_SquareVA.reset(VertexArray::Create());
		std::shared_ptr<VertexBuffer> squareVB;
		squareVB.reset(VertexBuffer::Create(verts2, sizeof(verts2)));

		squareVB->SetLayout(layout);
		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t indices2[3 * 2] = { 
			0, 1, 2,
			2, 3, 0
		};
		std::shared_ptr<IndexBuffer> m_IB2;
		m_IB2.reset(IndexBuffer::Create(indices2, sizeof(indices2) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(m_IB2);

		std::string vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_VP;

			out vec4 v_Color;
			
			void main()
			{
				v_Color = a_Color;
				gl_Position = u_VP * vec4(a_Position, 1.0);
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			in vec4 v_Color;
			
			void main()
			{
				color = v_Color;
			}
		)";

		m_Shader = std::make_unique<Shader>(vertexSrc, fragmentSrc);
	}

	Application::~Application()
	{
	}
	
	void Application::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));

//		EIS_CORE_TRACE("{0}", event);

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			(*--it)->OnEvent(event);
			if (event.m_Handled)
				break;
		}
	}

	void Application::Run()
	{
		while (m_Running)
		{
			float step = m_Rotation + 0.002f;

			RenderCommands::SetClearColor(glm::vec4(0.1f, 0.5f, 0.1f, 1.0f));
			RenderCommands::Clear();

			m_Camera.SetRotation(m_Rotation);

			Renderer::BeginScene(m_Camera);

			Renderer::Submit(m_Shader, m_SquareVA);
			Renderer::Submit(m_Shader, m_VA);

			Renderer::EndScene();

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();


			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			ImGuiIO io = ImGui::GetIO();
			m_Rotation += step * io.DeltaTime;
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	bool Application::OnWindowClose(WindowCloseEvent event)
	{
		m_Running = false;
		return true;
	}
}