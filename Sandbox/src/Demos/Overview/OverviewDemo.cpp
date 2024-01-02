#include "OverviewDemo.h"

OverviewDemo::OverviewDemo(const std::string& name) : Demo(name), m_CameraController(16.0f / 9.0f), m_LineAngle(0.0f)
{}

void OverviewDemo::Init()
{
	EIS_TRACE("Loading assets...");
	ice = Eis::Texture2D::Create("assets/textures/ice.png");
	mouce = Eis::Texture2D::Create("assets/textures/mouce.png");
	EIS_TRACE("Done loading assets.");

	Eis::RenderCommands::Disable(0x0B71); // GL_DEPTH_TEST
}

void OverviewDemo::Update(Eis::TimeStep ts)
{
	m_CameraController.OnUpdate(ts);

	{
		EIS_PROFILE_SCOPE("Renderer Prep");
		Eis::RenderCommands::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Eis::RenderCommands::Clear();
	}

	{
		EIS_PROFILE_SCOPE("Renderer Draw");

		Eis::Renderer2D::BeginScene(m_CameraController.GetCamera());

		static float rot = 0.0f; rot += 0.2f;
		if (rot >= 360.f) rot -= 360.0f;

		Eis::Renderer2D::DrawRotatedQuad(glm::vec2(2.0f, 1.0f), glm::vec2(1.0f, 0.3f), glm::radians(rot), glm::vec4(0.8f, 0.5f, 0.2f, 1.0f));
		Eis::Renderer2D::DrawQuad(glm::vec2(-1.0f, 0.0f), glm::vec2(1.0f), ice);

		static float x = 0.0f, inc = 0.01f;
		Eis::Renderer2D::DrawCircle(glm::vec2(x += inc, 1.0f), glm::vec2(1.0f), glm::vec4(1.0f));
		if (x > 2.0f || x < -2.0f) inc *= -1;


		Eis::Renderer2D::DrawLine(glm::vec2(1.0f, 0.0f), m_LineAngle, 0.5f, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), 1.5f);
		m_LineAngle += 0.15f;
		if (m_LineAngle > 360.0f)
			m_LineAngle -= 360.0f;

		Eis::Renderer2D::EndScene();
	}
}

void OverviewDemo::ImGuiRender()
{
	ImGui::Begin(m_Name.c_str());

	ImGui::Text("Line Angle: %.1f", m_LineAngle);
	ImGui::Text("Mouse Pos: %.3f, %.3f", m_CameraController.CalculateMouseWorldPos().x, m_CameraController.CalculateMouseWorldPos().y);

	ImGui::End();
}

void OverviewDemo::OnEvent(Eis::Event& e)
{
	m_CameraController.OnEvent(e);
}