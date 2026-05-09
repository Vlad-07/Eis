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
		m_HierarchyPanel.SetScene(m_ActiveScene);

		Entity cam = m_ActiveScene->CreateEntity("Camera");
		cam.AddComponent<CameraComponent>();

		Entity square = m_ActiveScene->CreateEntity("Square");
		square.AddComponent<SpriteRendererComponent>();
	}

	void EditorLayer::Detach()
	{
	}


	void EditorLayer::Update()
	{
		// Resize
		if (m_ViewportSize.x > 0 && m_ViewportSize.y > 0
			&& (m_Framebuffer->GetSpec().Width != m_ViewportSize.x
			|| m_Framebuffer->GetSpec().Height != m_ViewportSize.y))
		{
			m_Framebuffer->Resize(m_ViewportSize.x, m_ViewportSize.y);

			m_ActiveScene->OnViewportResize(m_ViewportSize.x, m_ViewportSize.y);
		}


		RenderCommands::SetClearColor(glm::vec3{});
		RenderCommands::Clear();

		m_Framebuffer->Bind();

		RenderCommands::SetClearColor(glm::vec3{ 35, 45, 61 } / 255.0f);
		RenderCommands::Clear();

		m_ActiveScene->OnUpdate();

		m_Framebuffer->Unbind();
	}

	void EditorLayer::ImGuiRender()
	{
		ImGuiStyle& style = ImGui::GetStyle();
		const float windowMinSizeX = style.WindowMinSize.x;
		style.WindowMinSize.x = 360.0f;

		ImGui::DockSpaceOverViewport(0u, nullptr, ImGuiDockNodeFlags_PassthruCentralNode);

		style.WindowMinSize.x = windowMinSizeX;


		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, glm::vec2{});
		ImGui::Begin("Viewport");

		m_ViewportSize = glm::vec2{ ImGui::GetContentRegionAvail() };

		uint64_t texId{ m_Framebuffer->GetColorAttachmentsIds()[0] };
		ImGui::Image(static_cast<ImTextureID>(texId), glm::vec2{ m_ViewportSize }, glm::vec2{ 0, 1 }, glm::vec2{ 1, 0 });

		ImGui::End();
		ImGui::PopStyleVar();


		m_HierarchyPanel.OnImGuiRender();
	}


	void EditorLayer::OnEvent(Event& event)
	{
	}
}