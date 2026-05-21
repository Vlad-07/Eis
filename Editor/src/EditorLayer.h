#pragma once

#include <Eis.h>

#include "Panels/Hierarchy.h"
#include "Panels/AssetBrowser.h"
#include "Eis/Rendering/Objects/EditorCamera.h"


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
		void NewScene();

		void SaveScene(const std::filesystem::path& path);
		void SaveSceneAs();

		void OpenScene();
		void OpenScene(const std::filesystem::path& path);


		void ScenePlay();
		void SceneStop();

		void UIToolbar();

		bool OnKeyPressed(KeyPressedEvent& e);
		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);

	private:

		enum class EditorState
		{
			EDIT = 0, PLAY = 1
		};

		EditorState m_State{ EditorState::EDIT };
		Ref<Scene> m_ActiveScene, m_EditedScene;
		std::filesystem::path m_EditedScenePath;

		Ref<Framebuffer> m_Framebuffer;

		HierarchyPanel m_HierarchyPanel;
		AssetBrowser m_AssetBrowserPanel;

		int m_GizmoType{ -1 };

		EditorCamera m_EditorCam;



		glm::uvec2 m_ViewportSize{};

		bool m_ViewportHovered{};
		bool m_ViewportFocused{};

		glm::ivec2 m_MousePosInViewport{};
	};
}