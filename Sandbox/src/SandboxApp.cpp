#include <Eis.h>

#include <imgui.h> // Should remove from sandbox aditional include dirs

class MainLayer : public Eis::Layer
{
private:
	std::shared_ptr<Eis::Shader> m_Shader;
	std::shared_ptr<Eis::VertexArray> m_VA;
	std::shared_ptr<Eis::VertexArray> m_SquareVA;

	Eis::OrthographicCamera m_Camera;
	glm::vec3 m_CamPos;
	float m_CameraSpeed;

	float m_DeltaTime = 0.0f;

public:
	MainLayer() : Layer("Sandbox"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CamPos(0.0f), m_CameraSpeed(1.0f)
	{
		m_VA.reset(Eis::VertexArray::Create());

		float verts[3 * 7] = {
			-0.5f, -0.5f, 0.0f,   0.9f, 0.1f, 0.1f, 0.9f,
			 0.5f, -0.5f, 0.0f,   0.1f, 0.9f, 0.1f, 0.9f,
			 0.0f,  0.5f, 0.0f,   0.1f, 0.1f, 0.9f, 0.9f
		};
		std::shared_ptr<Eis::VertexBuffer> VB;
		VB.reset(Eis::VertexBuffer::Create(verts, sizeof(verts)));

		Eis::BufferLayout layout = {
			{ Eis::ShaderDataType::Float3, "a_Position" },
			{ Eis::ShaderDataType::Float4, "a_Color" }
		};
		VB->SetLayout(layout);
		m_VA->AddVertexBuffer(VB);

		uint32_t indices[3] = { 0, 1, 2 };
		std::shared_ptr<Eis::IndexBuffer> IB;
		IB.reset(Eis::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VA->SetIndexBuffer(IB);


		float verts2[4 * 7] = {
			-0.75f, -0.75f, 0.0f,   0.1f, 0.1f, 0.8f, 1.0f,
			 0.75f, -0.75f, 0.0f,   0.1f, 0.9f, 0.1f, 1.0f,
			 0.75f,  0.75f, 0.0f,   0.1f, 0.1f, 0.9f, 1.0f,
			-0.75f,  0.75f, 0.0f,   0.9f, 0.1f, 0.1f, 1.0f
		};
		m_SquareVA.reset(Eis::VertexArray::Create());
		std::shared_ptr<Eis::VertexBuffer> squareVB;
		squareVB.reset(Eis::VertexBuffer::Create(verts2, sizeof(verts2)));

		squareVB->SetLayout(layout);
		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t indices2[3 * 2] = {
			0, 1, 2,
			2, 3, 0
		};
		std::shared_ptr<Eis::IndexBuffer> m_IB2;
		m_IB2.reset(Eis::IndexBuffer::Create(indices2, sizeof(indices2) / sizeof(uint32_t)));
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

		m_Shader = std::make_unique<Eis::Shader>(vertexSrc, fragmentSrc);
	}

	virtual void OnUpdate()
	{
		Eis::RenderCommands::SetClearColor(glm::vec4(m_CamPos.x / 10, m_CamPos.y / 10, 0.2f, 1.0f));
		Eis::RenderCommands::Clear();

		if (Eis::Input::IsKeyPressed(EIS_KEY_LEFT))
			m_CamPos.x -= m_CameraSpeed * m_DeltaTime;

		if (Eis::Input::IsKeyPressed(EIS_KEY_RIGHT))
			m_CamPos.x += m_CameraSpeed * m_DeltaTime;

		if (Eis::Input::IsKeyPressed(EIS_KEY_UP))
			m_CamPos.y += m_CameraSpeed * m_DeltaTime;

		if (Eis::Input::IsKeyPressed(EIS_KEY_DOWN))
			m_CamPos.y -= m_CameraSpeed * m_DeltaTime;


		m_Camera.SetPosition(m_CamPos);

		Eis::Renderer::BeginScene(m_Camera);

		Eis::Renderer::Submit(m_Shader, m_SquareVA);
		Eis::Renderer::Submit(m_Shader, m_VA);

		Eis::Renderer::EndScene();
	}

	virtual void OnImGuiRender()
	{
		ImGuiIO io = ImGui::GetIO();
		m_DeltaTime = io.DeltaTime;
		
		ImGui::SliderFloat("Camera speed", &m_CameraSpeed, 0.0f, 50.0f);
	}

	virtual void OnEvent(Eis::Event& event)
	{
//		Eis::EventDispatcher dispatcher(event);
//		dispatcher.Dispatch<Eis::KeyPressedEvent>(EIS_BIND_EVENT_FN(MainLayer::OnKeyPressedEvent));
	}

	bool OnKeyPressedEvent(Eis::KeyPressedEvent& event)
	{
		if (event.GetKeyCode() == EIS_KEY_LEFT)
			m_CamPos.x -= m_CameraSpeed * m_DeltaTime;

		if (event.GetKeyCode() == EIS_KEY_RIGHT)
			m_CamPos.x += m_CameraSpeed * m_DeltaTime;

		if (event.GetKeyCode() == EIS_KEY_UP)
			m_CamPos.y += m_CameraSpeed * m_DeltaTime;

		if (event.GetKeyCode() == EIS_KEY_DOWN)
			m_CamPos.y -= m_CameraSpeed * m_DeltaTime;

		return false;
	}
};


class Sandbox : public Eis::Application
{
public:

	Sandbox()
	{
		PushLayer(new MainLayer());
	}

	~Sandbox()
	{
	}
};

Eis::Application* Eis::CreateApplication()
{
	return new Sandbox();
}