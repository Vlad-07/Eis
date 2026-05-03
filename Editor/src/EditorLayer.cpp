#include "EditorLayer.h"
#include <imgui.h>


namespace Eis
{
	EditorLayer::EditorLayer()
		: Layer{ "EditorLayer" }
	{}


	void EditorLayer::Attach()
	{
		FramebufferSpec fbSpec;
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		fbSpec.AttachmentsSpec = { { FramebufferTexFormat::RGBA8 } };
		m_Framebuffer = Framebuffer::Create(fbSpec);

		m_ActiveScene = CreateRef<Scene>();
	}

	void EditorLayer::Detach()
	{
	}


	void EditorLayer::Update()
	{
		m_CameraController.Update();
	}

	void EditorLayer::Render()
	{
		// Resize framebuffer
		if (m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f
			&& m_Framebuffer->GetSpec().Width != m_ViewportSize.x
			&& m_Framebuffer->GetSpec().Height != m_ViewportSize.y)
		{
			m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);

			WindowResizeEvent e{ (int32_t)m_ViewportSize.x, (int32_t)m_ViewportSize.y };
			m_CameraController.OnEvent(e);
		}


		Renderer2D::SetClearColor(glm::vec3{});
		Renderer2D::Clear();

		m_Framebuffer->Bind();

		Renderer2D::SetClearColor(glm::vec3{ 35, 45, 61 } / 255.0f);
		Renderer2D::Clear();


		Renderer2D::BeginScene(m_CameraController.GetCamera());

		Renderer2D::DrawQuad(glm::vec2{}, glm::vec2{ 1.0f }, glm::vec4{1.0f});

		Renderer2D::EndScene();

		m_Framebuffer->Unbind();
	}

	void EditorLayer::ImGuiRender()
	{
		ImGui::DockSpaceOverViewport(0u, nullptr, ImGuiDockNodeFlags_PassthruCentralNode);


		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, glm::vec2{});
		ImGui::Begin("Viewport");

		m_ViewportSize = ImGui::GetContentRegionAvail();

		uint32_t texId{ m_Framebuffer->GetColorAttachmentsIds()[0] };
		ImGui::Image(texId, m_ViewportSize, glm::vec2{0, 1}, glm::vec2{1, 0});

		ImGui::End();
		ImGui::PopStyleVar();
	}


	void EditorLayer::OnEvent(Event& event)
	{
		// change camera according to viewport, not window
		if (event.GetEventType() != EventType::WindowResize)
			m_CameraController.OnEvent(event);
	}
}