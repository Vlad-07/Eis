#include "DemoLayer.h"


DemoLayer::DemoLayer() : Layer("Demo"), m_DemoManager()
{}

void DemoLayer::OnAttach()
{
	EIS_PROFILE_FUNCTION();

	Eis::RenderCommands::Disable(0x0B71); // GL_DEPTH_TEST

	m_DemoManager.LoadDemo(new OverviewDemo("Overview"));
	m_DemoManager.LoadDemo(new GTDemo("Game Theory"));

	m_DemoManager.GetCurrentDemo().Init();
}

void DemoLayer::OnDetach()
{
	EIS_PROFILE_FUNCTION();
}


void DemoLayer::OnUpdate(Eis::TimeStep ts)
{
	EIS_PROFILE_FUNCTION();

	m_DemoManager.GetCurrentDemo().Update(ts);
}

void DemoLayer::OnImGuiRender()
{
	EIS_PROFILE_FUNCTION();

	ImGui::Begin("Demo Menu");
	for (int i = 0; i < m_DemoManager.GetDemos().size(); i++)
	{
		if (ImGui::Button(m_DemoManager.GetDemos()[i]->GetName().c_str()) && i != m_DemoManager.GetCurrentDemoId())
		{
			m_DemoManager.SetCurrentDemo(i);
			m_DemoManager.GetCurrentDemo().Init();
			EIS_INFO("Selected {0} demo", m_DemoManager.GetCurrentDemo().GetName());
		}
	}
	ImGui::End();

	m_DemoManager.GetCurrentDemo().ImGuiRender();
}

void DemoLayer::OnEvent(Eis::Event& e)
{
	EIS_PROFILE_FUNCTION();

	m_DemoManager.GetCurrentDemo().OnEvent(e);
}