#include <Eis.h>
#include <Eis/Core/EntryPoint.h>

#include "imgui.h" // Should remove from sandbox aditional include dirs

#include <glm/gtc/matrix_transform.hpp>

#include "Platform/OpenGL/OpenGLShader.h"
#include "Platform/OpenGL/OpenGLTexture.h"

#include "Sandbox2D.h"

class MainLayer : public Eis::Layer
{
private:
	Eis::ShaderLibrary m_ShaderLibrary;
	Eis::Ref<Eis::Shader> m_Shader;
	Eis::Ref<Eis::VertexArray> m_VA;
	Eis::Ref<Eis::VertexArray> m_SquareVA;

	Eis::Ref<Eis::Texture2D> m_Texture, m_MouceTexture;

	Eis::OrthoCameraController m_CameraController;

	float m_Spacing = 0.2f;

	int frame = 0;

public:
	MainLayer()
		: Layer("Sandbox"),
		m_CameraController(16.0f / 9.0f)
	{
		m_VA = Eis::VertexArray::Create();

		float verts[5 * 4] = {
			-0.5f, -0.5f, 0.0f,  0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f,  1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f,  1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f,  0.0f, 1.0f
		};
		Eis::Ref<Eis::VertexBuffer> VB;
		VB = Eis::VertexBuffer::Create(verts, sizeof(verts));

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
		IB = Eis::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
		m_VA->SetIndexBuffer(IB);


		float verts2[3 * 4] = {
			-0.75f, -0.75f, 0.0f,
			 0.75f, -0.75f, 0.0f,
			 0.75f,  0.75f, 0.0f,
			-0.75f,  0.75f, 0.0f
		};
		m_SquareVA = Eis::VertexArray::Create();
		Eis::Ref<Eis::VertexBuffer> squareVB;
		squareVB = Eis::VertexBuffer::Create(verts2, sizeof(verts2));

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
		m_IB2 = Eis::IndexBuffer::Create(indices2, sizeof(indices2) / sizeof(uint32_t));
		m_SquareVA->SetIndexBuffer(m_IB2);

		

		m_Shader = Eis::Shader::Create("assets/shaders/FlatColor.glsl");

		auto textureShader = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");


		m_Texture = Eis::Texture2D::Create("assets/textures/ice.png");
		std::dynamic_pointer_cast<Eis::OpenGLShader>(textureShader)->Bind();
		std::dynamic_pointer_cast<Eis::OpenGLShader>(textureShader)->UploadUniformInt("u_Texture", 0);

		m_MouceTexture = Eis::Texture2D::Create("assets/textures/mouce.png");
}

	virtual void OnUpdate(Eis::TimeStep ts) override
	{
		if (Eis::Input::IsKeyPressed(EIS_KEY_ESCAPE))
			exit(0);

		m_CameraController.OnUpdate(ts);

		if (frame % 72 == 0)
		{
			EIS_TRACE("Frametime: {0}s ({1}ms)", ts.GetSeconds(), ts.GetMilliseconds());
			frame = 1;
		}
		frame++;

		Eis::RenderCommands::SetClearColor(glm::vec4(m_CameraController.GetCamera().GetPosition().x / 10, m_CameraController.GetCamera().GetPosition().y / 10, 0.2f, 1.0f));
		Eis::RenderCommands::Clear();

		Eis::Renderer::BeginScene(m_CameraController.GetCamera());

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

		auto textureShader = m_ShaderLibrary.Get("Texture");

		m_Texture->Bind();
		Eis::Renderer::Submit(textureShader, m_VA, glm::scale(glm::mat4(1.0f), glm::vec3(1.2f)));

//		m_MouceTexture->Bind();
//		Eis::Renderer::Submit(textureShader, m_VA, glm::scale(glm::mat4(1.0f), glm::vec3(1.2f)));

		Eis::Renderer::EndScene();
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Controls");
		ImGui::SliderFloat("Spacing", &m_Spacing, 0.0f, 1.0f);
		ImGui::End();
	}

	virtual void OnEvent(Eis::Event& e) override
	{
		m_CameraController.OnEvent(e);
	}
};

class Sandbox : public Eis::Application
{
public:

	Sandbox()
	{
		PushLayer(new Sandbox2D());
	}

	~Sandbox() = default;
};

Eis::Application* Eis::CreateApplication()
{
	return new Sandbox();
}