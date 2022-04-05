#include "Sandbox2D.h"

#include "imgui.h"

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(16.0f / 9.0f)
{
	
}

void Sandbox2D::OnAttach()
{
	m_VA = Eis::VertexArray::Create();

	float verts2[3 * 4] = {
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f,
			-0.5f, -0.5f, 0.0f
	};
	m_VA = Eis::VertexArray::Create();
	Eis::Ref<Eis::VertexBuffer> VB;
	VB = Eis::VertexBuffer::Create(verts2, sizeof(verts2));

	Eis::BufferLayout layout = {
		{ Eis::ShaderDataType::Float3, "a_Position" }
	};
	VB->SetLayout(layout);
	m_VA->AddVertexBuffer(VB);

	uint32_t indices[3 * 2] = { 0, 1, 2, 2, 3, 0 };
	Eis::Ref<Eis::IndexBuffer> IB;
	IB = Eis::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
	m_VA->SetIndexBuffer(IB);

	m_FlatColorShader = Eis::Shader::Create("assets/shaders/FlatColor.glsl");
}

void Sandbox2D::OnDetach()
{

}

void Sandbox2D::OnUpdate(Eis::TimeStep ts)
{
	m_CameraController.OnUpdate(ts);

	Eis::RenderCommands::SetClearColor(glm::vec4(m_CameraController.GetCamera().GetPosition().x / 10, m_CameraController.GetCamera().GetPosition().y / 10, 0.2f, 1.0f));
	Eis::RenderCommands::Clear();

	Eis::Renderer::BeginScene(m_CameraController.GetCamera());

	std::dynamic_pointer_cast<Eis::OpenGLShader>(m_FlatColorShader)->Bind();
	std::dynamic_pointer_cast<Eis::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat3("u_Color", glm::vec3(0.2f, 0.8f, 0.3f));

	Eis::Renderer::Submit(m_FlatColorShader, m_VA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

	Eis::Renderer::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Controls");

	ImGui::End();
}

void Sandbox2D::OnEvent(Eis::Event& e)
{
	m_CameraController.OnEvent(e);
}