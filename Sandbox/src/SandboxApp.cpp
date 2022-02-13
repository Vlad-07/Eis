#include <Eis.h>

#include <imgui.h> // Should remove from sandbox aditional include dirs

#include <glm/gtc/matrix_transform.hpp>

#include "Platform/OpenGL/OpenGLShader.h"

class MainLayer : public Eis::Layer
{
private:
	Eis::Ref<Eis::Shader> m_Shader;
	Eis::Ref<Eis::VertexArray> m_VA;
	Eis::Ref<Eis::VertexArray> m_SquareVA;

	Eis::OrthographicCamera m_Camera;
	glm::vec3 m_CamPos;
	float m_CameraRotation;
	float m_CameraSpeed;
	float m_CameraRotationSpeed;

	float m_Spacing = 0.2f;
	glm::vec4 color = glm::vec4(0.4f, 0.9f, 0.7f, 1.0f);

public:
	MainLayer()
		: Layer("Sandbox"),
		m_Camera(-1.6f, 1.6f, -0.9f, 0.9f),
		m_CamPos(0.0f),
		m_CameraSpeed(1.0f),
		m_CameraRotation(0.0f),
		m_CameraRotationSpeed(35.0f)
	{
		m_VA.reset(Eis::VertexArray::Create());

		float verts[3 * 3] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.0f,  0.5f, 0.0f
		};
		Eis::Ref<Eis::VertexBuffer> VB;
		VB.reset(Eis::VertexBuffer::Create(verts, sizeof(verts)));

		Eis::BufferLayout layout = {
			{ Eis::ShaderDataType::Float3, "a_Position" }
		};
		VB->SetLayout(layout);
		m_VA->AddVertexBuffer(VB);

		uint32_t indices[3] = { 0, 1, 2 };
		Eis::Ref<Eis::IndexBuffer> IB;
		IB.reset(Eis::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VA->SetIndexBuffer(IB);


		float verts2[3 * 4] = {
			-0.75f, -0.75f, 0.0f,
			 0.75f, -0.75f, 0.0f,
			 0.75f,  0.75f, 0.0f,
			-0.75f,  0.75f, 0.0f
		};
		m_SquareVA.reset(Eis::VertexArray::Create());
		Eis::Ref<Eis::VertexBuffer> squareVB;
		squareVB.reset(Eis::VertexBuffer::Create(verts2, sizeof(verts2)));

		squareVB->SetLayout(layout);
		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t indices2[3 * 2] = {
			0, 1, 2,
			2, 3, 0
		};
		Eis::Ref<Eis::IndexBuffer> m_IB2;
		m_IB2.reset(Eis::IndexBuffer::Create(indices2, sizeof(indices2) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(m_IB2);

		std::string vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_VP;
			uniform mat4 u_Transform;
			uniform vec4 u_Color;

			out vec4 v_Color;
			
			void main()
			{
				v_Color = u_Color;
				gl_Position = u_VP * u_Transform * vec4(a_Position, 1.0);
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

		m_Shader.reset(Eis::Shader::Create(vertexSrc, fragmentSrc));
	}

	virtual void OnUpdate(Eis::TimeStep ts) override
	{
		float deltaTime = ts;

		EIS_TRACE("Frametime: {0}s ({1}ms)", ts.GetSeconds(), ts.GetMilliseconds());

		Eis::RenderCommands::SetClearColor(glm::vec4(m_CamPos.x / 10, m_CamPos.y / 10, 0.2f, 1.0f));
		Eis::RenderCommands::Clear();

		if (Eis::Input::IsKeyPressed(EIS_KEY_LEFT) || Eis::Input::IsKeyPressed(EIS_KEY_A))
			m_CamPos.x -= m_CameraSpeed * deltaTime;

		if (Eis::Input::IsKeyPressed(EIS_KEY_RIGHT) || Eis::Input::IsKeyPressed(EIS_KEY_D))
			m_CamPos.x += m_CameraSpeed * deltaTime;

		if (Eis::Input::IsKeyPressed(EIS_KEY_UP) || Eis::Input::IsKeyPressed(EIS_KEY_W))
			m_CamPos.y += m_CameraSpeed * deltaTime;

		if (Eis::Input::IsKeyPressed(EIS_KEY_DOWN) || Eis::Input::IsKeyPressed(EIS_KEY_S))
			m_CamPos.y -= m_CameraSpeed * deltaTime;

		if (Eis::Input::IsKeyPressed(EIS_KEY_Q))
			m_CameraRotation += m_CameraRotationSpeed * deltaTime;

		if (Eis::Input::IsKeyPressed(EIS_KEY_E))
			m_CameraRotation -= m_CameraRotationSpeed * deltaTime;

		m_Camera.SetPosition(m_CamPos);
		m_Camera.SetRotation(m_CameraRotation);

		Eis::Renderer::BeginScene(m_Camera);

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		std::dynamic_pointer_cast<Eis::OpenGLShader>(m_Shader)->Bind();

		for (int x = 0; x < 10; x++)
		{
			for (int y = 0; y < 10; y++)
			{
				if (x % 2 == 0)
					std::dynamic_pointer_cast<Eis::OpenGLShader>(m_Shader)->UploadUniformFloat4("u_Color", glm::vec4(0.2f, 0.8f, 0.3f, 1.0f));
				else
					std::dynamic_pointer_cast<Eis::OpenGLShader>(m_Shader)->UploadUniformFloat4("u_Color", glm::vec4(0.8f, 0.2f, 0.3f, 1.0f));

				glm::vec3 pos(y * m_Spacing, x * m_Spacing, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				Eis::Renderer::Submit(m_Shader, m_SquareVA, transform);
			}
		}
		std::dynamic_pointer_cast<Eis::OpenGLShader>(m_Shader)->UploadUniformFloat4("u_Color", color);
		Eis::Renderer::Submit(m_Shader, m_VA);
		Eis::Renderer::EndScene();
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Controls");
		ImGui::SliderFloat("Camera speed", &m_CameraSpeed, 1.0f, 50.0f);
		ImGui::SliderFloat("Spacing", &m_Spacing, 0.0f, 1.0f);
		ImGui::ColorEdit3("Color", (float*)&color);
		ImGui::End();
	}

	virtual void OnEvent(Eis::Event& event) override
	{
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