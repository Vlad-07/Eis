#pragma once

#include "Eis.h"

#include "Eis/Rendering/Objects/EditorCamera2.h"

#include "Panels/Hierarchy.h"
#include "Panels/AssetBrowser.h"


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
		void NewProject();
		void SaveProject();
		void OpenProject(const std::filesystem::path& path);


		void NewScene();

		void SaveScene();
		void SaveSceneAs();

		void OpenScene();
		void OpenScene(AssetHandle handle);


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

		std::filesystem::path m_ProjectPath;

		EditorState m_State{ EditorState::EDIT };
		Ref<Scene> m_ActiveScene, m_EditedScene;
		std::filesystem::path m_EditedScenePath;

		Ref<Framebuffer> m_Framebuffer;

		Scope<HierarchyPanel> m_HierarchyPanel;
		Scope<AssetBrowser> m_AssetBrowserPanel;

		int m_GizmoType{ -1 };

		EditorCamera2 m_EditorCam;


		bool m_ViewportHovered{};
		bool m_ViewportFocused{};


		glm::uvec2 m_ViewportSize{};
		glm::vec2 m_ViewportScreenPos{};
		glm::ivec2 m_MousePosInViewport{};


		Ref<Texture2D> m_PlayIcon, m_StopIcon;
	};
}