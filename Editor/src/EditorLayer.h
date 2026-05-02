#pragma once

#include <Eis.h>


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
		virtual void Render() override;
		virtual void ImGuiRender() override;

		virtual void OnEvent(Event& event) override;

	private:
		OrthoCameraController m_CameraController;

		Ref<Scene> m_ActiveScene;

		Ref<Framebuffer> m_Framebuffer;
	};
}