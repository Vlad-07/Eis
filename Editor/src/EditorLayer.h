#pragma once

#include <Eis.h>

#include "Panels/Hierarchy.h"


namespace Eis
{
	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void Attach() override;
		virtual void Detach() override;

		virtual void Update() override;
		virtual void ImGuiRender() override;

		virtual void OnEvent(Event& event) override;

	private:
		Ref<Scene> m_ActiveScene;

		Ref<Framebuffer> m_Framebuffer;
		glm::uvec2 m_ViewportSize{};

		HierarchyPanel m_HierarchyPanel;
	};
}