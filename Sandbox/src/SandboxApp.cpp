#include <Eis.h>

#include <imgui.h> // Should remove from sandbox aditional include dirs

#include <glm/gtc/matrix_transform.hpp>

#include "Platform/OpenGL/OpenGLShader.h"
#include "Platform/OpenGL/OpenGLTexture.h"

class MainLayer : public Eis::Layer
{
private:
	Eis::Ref<Eis::Shader> m_Shader, m_TextureShader;
	Eis::Ref<Eis::VertexArray> m_VA;
	Eis::Ref<Eis::VertexArray> m_SquareVA;

	Eis::Ref<Eis::Texture2D> m_Texture, m_MouceTexture;

	Eis::OrthographicCamera m_Camera;
	glm::vec3 m_CamPos;
	float m_CameraRotation;
	float m_CameraSpeed;
	float m_CameraRotationSpeed;

	float m_Spacing;

public:
	MainLayer()
		: Layer("Sandbox"),
		m_Camera(-1.6f, 1.6f, -0.9f, 0.9f),
		m_CamPos(0.0f),
		m_CameraSpeed(1.0f),
		m_CameraRotation(0.0f),
		m_CameraRotationSpeed(35.0f),
		m_Spacing(0.2f)
	{
		m_VA.reset(Eis::VertexArray::Create());

		float verts[5 * 4] = {
			-0.5f, -0.5f, 0.0f,  0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f,  1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f,  1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f,  0.0f, 1.0f
		};
		Eis::Ref<Eis::VertexBuffer> VB;
		VB.reset(Eis::VertexBuffer::Create(verts, sizeof(verts)));

		Eis::BufferLayout layout = {
			{ Eis::ShaderDataType::Float3, "a_Position" },
			{ Eis::ShaderDataType::Float2, "a_TexCoord" }
		};
		VB->SetLayout(layout);
		m_VA->AddVertexBuffer(VB);

		uint32_t indices[3 * 2] = {
			0, 1, 2,
			2, 3, 0
		};
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

		Eis::BufferLayout layout2 = {
			{ Eis::ShaderDataType::Float3, "a_Position" }
		};
		squareVB->SetLayout(layout2);
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
			uniform vec3 u_Color;

			out vec3 v_Color;
			
			void main()
			{
				v_Color = u_Color;
				gl_Position = u_VP * u_Transform * vec4(a_Position, 1.0);
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			in vec3 v_Color;
			
			void main()
			{
				color = vec4(v_Color, 1.0f);
			}
		)";

		m_Shader.reset(Eis::Shader::Create(vertexSrc, fragmentSrc));


		std::string vertexSrcTex = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec2 a_TexCoord;

			uniform mat4 u_VP;
			uniform mat4 u_Transform;

			out vec2 v_TexCoord;
			
			void main()
			{
				v_TexCoord = a_TexCoord;
				gl_Position = u_VP * u_Transform * vec4(a_Position, 1.0);
			}
		)";

		std::string fragmentSrcTex = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			in vec2 v_TexCoord;
			uniform sampler2D u_Texture;
			
			void main()
			{
				color = texture(u_Texture, v_TexCoord);
			}
		)";

		m_TextureShader.reset(Eis::Shader::Create(vertexSrcTex, fragmentSrcTex));

		m_Texture = Eis::Texture2D::Create("assets/textures/ice.png");
		std::dynamic_pointer_cast<Eis::OpenGLShader>(m_TextureShader)->Bind();
		std::dynamic_pointer_cast<Eis::OpenGLShader>(m_TextureShader)->UploadUniformInt("u_Texture", 0);

		m_MouceTexture = Eis::Texture2D::Create("assets/textures/mouce.png");
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
					std::dynamic_pointer_cast<Eis::OpenGLShader>(m_Shader)->UploadUniformFloat3("u_Color", glm::vec3(0.2f, 0.8f, 0.3f));
				else
					std::dynamic_pointer_cast<Eis::OpenGLShader>(m_Shader)->UploadUniformFloat3("u_Color", glm::vec3(0.8f, 0.2f, 0.3f));

				glm::vec3 pos(y * m_Spacing, x * m_Spacing, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				Eis::Renderer::Submit(m_Shader, m_SquareVA, transform);
			}
		}

		m_Texture->Bind();
		Eis::Renderer::Submit(m_TextureShader, m_VA, glm::scale(glm::mat4(1.0f), glm::vec3(1.2f)));

		m_MouceTexture->Bind();
		Eis::Renderer::Submit(m_TextureShader, m_VA, glm::scale(glm::mat4(1.0f), glm::vec3(1.2f)));

		Eis::Renderer::EndScene();
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Controls");
		ImGui::SliderFloat("Camera speed", &m_CameraSpeed, 1.0f, 50.0f);
		ImGui::SliderFloat("Spacing", &m_Spacing, 0.0f, 1.0f);
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