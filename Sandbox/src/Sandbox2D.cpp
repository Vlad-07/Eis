#include "Sandbox2D.h"

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(16.0f / 9.0f)
{
}

void Sandbox2D::OnAttach()
{
	EIS_TRACE("Loading assets...");

	ice = Eis::Texture2D::Create("assets/textures/ice.png");
	mouce = Eis::Texture2D::Create("assets/textures/mouce.png");
//	map = Eis::Texture2D::Create("assets/textures/10k.png");
	EIS_TRACE("Done loading assets");
}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(Eis::TimeStep ts)
{
	m_CameraController.OnUpdate(ts);

	Eis::RenderCommands::SetClearColor({0.1f, 0.1f, 0.1f, 1.0f});
	Eis::RenderCommands::Clear();

	Eis::Renderer2D::BeginScene(m_CameraController.GetCamera());

	Eis::Renderer2D::DrawQuad({ 2.0f, 1.0f }, { 1.0f, 0.3f }, { 0.8f, 0.5f, 0.2f, 1.0f });
	Eis::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, ice);
//	Eis::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 20.0f, 10.0f }, map);

	static float x = 0.0f, inc = 0.01f;
	Eis::Renderer2D::DrawCircle({ x += inc, 1.0f }, glm::vec2(1.0f), mouce);
	if (x > 2.0f || x < -2.0f) inc *= -1;

	Eis::Renderer2D::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
}

void Sandbox2D::OnEvent(Eis::Event& e)
{
	m_CameraController.OnEvent(e);
}