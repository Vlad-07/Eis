#include "EditorLayer.h"

#include "Eis/Assets/Importers/TextureImporter.h"
#include "Eis/Assets/Importers/SceneImporter.h"
#include "Eis/Utils/PlatformUtils.h"

#include <imgui.h>
#include <imgui_internal.h>

#include <ImGuizmo.h>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>



namespace Eis
{
	EditorLayer::EditorLayer()
		: Layer{ "EditorLayer" }
	{}


	void EditorLayer::Attach()
	{
		m_PlayIcon = TextureImporter::LoadTexture2D("resources/icons/play.png");
		m_StopIcon = TextureImporter::LoadTexture2D("resources/icons/stop.png");

		FramebufferSpec fbSpec;
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		fbSpec.AttachmentsSpec = {
			{ FramebufferTexFormat::RGBA8, FramebufferClearValue{ glm::vec4{ 35, 45, 61, 255 } / 255.0f} },
			{ FramebufferTexFormat::R32I, FramebufferClearValue{ -1 } }
		};
		m_Framebuffer = Framebuffer::Create(fbSpec);

		m_HierarchyPanel = CreateScope<HierarchyPanel>();

		m_ProjectPath = FileDialogs::OpenFile("Eis Project (.eproj)\0*.eproj\0");
		if (m_ProjectPath.empty())
			EIS_CORE_ASSERT(false, "TODO: project creation UI");
		OpenProject(m_ProjectPath);

		m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
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

			m_EditorCam.SetViewportSize((float)m_ViewportSize.x, (float)m_ViewportSize.y);
		}

		if (m_State == EditorState::EDIT)
			m_EditorCam.OnUpdate();

		RenderCommands::SetClearColor(glm::vec3{});
		RenderCommands::Clear();

		m_Framebuffer->Bind();
		m_Framebuffer->Clear();

		if (m_State == EditorState::EDIT)
			m_ActiveScene->OnUpdateEditor(m_EditorCam);
		else if (m_State == EditorState::PLAY)
			m_ActiveScene->OnUpdateRuntime();

		m_Framebuffer->Unbind();
	}

	void EditorLayer::ImGuiRender()
	{
		ImGui::PushStyleVarX(ImGuiStyleVar_WindowMinSize, 150);
		ImGui::DockSpaceOverViewport(0u, nullptr, ImGuiDockNodeFlags_PassthruCentralNode);
		ImGui::PopStyleVar();

		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New Scene", "Ctrl+Shift+N"))
					NewScene();
				if (ImGui::MenuItem("Open Scene...", "Ctrl+O"))
					OpenScene();

				ImGui::Separator();

				if (ImGui::MenuItem("Save", "Ctrl+S"))
					SaveScene();
				if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
					SaveSceneAs();

				ImGui::Separator();

				if (ImGui::MenuItem("Exit", "Alt+F4"))
					Application::Get().Close();

				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}


		// Viewport Window
		{
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, glm::vec2{});
			ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_MenuBar);

			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("Cam"))
				{
					ImGui::Text("TODO");
					// TODO: better editor camera
					// TODO: camera settings

					ImGui::EndMenu();
				}

				ImGui::EndMenuBar();
			}


			m_ViewportHovered = ImGui::IsWindowHovered();
			m_ViewportFocused = ImGui::IsWindowFocused();

			m_ViewportSize = glm::vec2{ ImGui::GetContentRegionAvail() };

			m_ViewportScreenPos = ImGui::GetCursorScreenPos();
			m_MousePosInViewport = glm::vec2{ ImGui::GetMousePos() } - m_ViewportScreenPos;
			// OpenGL has (0,0) at bottom left, ImGui at top left
			m_MousePosInViewport.y = m_ViewportSize.y - m_MousePosInViewport.y;

			// View
			uint64_t texId{ m_Framebuffer->GetColorAttachmentsIds()[0] };
			ImGui::Image(static_cast<ImTextureID>(texId), glm::vec2{ m_ViewportSize }, { 0,1 }, { 1,0 });

			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_DRAG"))
				{
					AssetHandle handle = *(AssetHandle*)payload->Data;
					OpenScene(handle);
				}

				ImGui::EndDragDropTarget();
			}

			// Gizmos
			{
				Entity selected = m_HierarchyPanel->GetSelectedEntity();
				if (selected && m_GizmoType != -1 && m_State == EditorState::EDIT)
				{
					ImGuizmo::SetOrthographic(false);
					ImGuizmo::SetDrawlist();

					ImGuizmo::SetRect(m_ViewportScreenPos.x, m_ViewportScreenPos.y, (float)m_ViewportSize.x, (float)m_ViewportSize.y);

					// Camera...
					glm::mat4 camProj = m_EditorCam.GetProjection();
					glm::mat4 camView = m_EditorCam.GetView();

					auto& tc = selected.GetComponent<TransformComponent>();
					glm::mat4 transform = tc.GetTransform();

					// Snap
					const bool snap = Input::IsKeyPressed(Key::LeftControl);
					const float snapVal = m_GizmoType == ImGuizmo::OPERATION::ROTATE ? 45.0f : 0.5f;
					const float snapVec[3]{ snapVal, snapVal, snapVal };

					ImGuizmo::Manipulate(glm::value_ptr(camView), glm::value_ptr(camProj),
						(ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::MODE::LOCAL, glm::value_ptr(transform),
						nullptr, snap ? snapVec : nullptr);

					if (ImGuizmo::IsUsing())
					{
						glm::quat rot{};
						glm::vec3 skew{};
						glm::vec4 perspective{};

						// shady function
						glm::decompose(transform, tc.Scale, rot, tc.Translation, skew, perspective);

						tc.Rotation = glm::eulerAngles(rot);
					}
				}
			}

			ImGui::End();
			ImGui::PopStyleVar();
		}

		UIToolbar();
		m_HierarchyPanel->OnImGuiRender();
		m_AssetBrowserPanel->OnImGuiRender();
	}

	void EditorLayer::UIToolbar()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, glm::vec2{ 0, 3 });
		ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, glm::vec2{});
		ImGui::PushStyleColor(ImGuiCol_Button, glm::vec4{});

		ImGui::Begin("##toolbar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollWithMouse);

		ImGui::DockBuilderGetNode(ImGui::GetWindowDockID())->LocalFlags |= ImGuiDockNodeFlags_NoTabBar;

		const glm::vec2 size{ ImGui::GetWindowHeight() - ImGui::GetStyle().WindowPadding.y * 3.0f }; // why 3?
		ImGui::SetCursorPosX((ImGui::GetContentRegionMax().x - size.x) * 0.5f);
		if (m_State == EditorState::EDIT)
		{
			if (ImGui::ImageButton("Play", m_PlayIcon->GetRendererId(), size))
				ScenePlay();
		}
		else if (m_State == EditorState::PLAY)
		{
			if (ImGui::ImageButton("Stop", m_StopIcon->GetRendererId(), size))
				SceneStop();
		}

		// TODO: pause button

		ImGui::End();
		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor();
	}



	void EditorLayer::NewProject()
	{
		// TODO: prompt for path, etc
		Project::New();
	}

	void EditorLayer::SaveProject()
	{
		Project::SaveActive(m_ProjectPath);
	}

	void EditorLayer::OpenProject(const std::filesystem::path& path)
	{
		if (Project::Load(path))
		{
			OpenScene(Project::GetActive()->GetConfig().StartingScene);
			m_AssetBrowserPanel = CreateScope<AssetBrowser>();
			m_ProjectPath = path;
		}
	}



	void EditorLayer::NewScene()
	{
		if (m_State != EditorState::EDIT)
			return;

		m_EditedScene = CreateRef<Scene>();
		SaveSceneAs();
	}

	void EditorLayer::SaveScene()
	{
		if (m_State != EditorState::EDIT)
			return;

		SceneImporter::SaveScene(m_EditedScene, m_EditedScenePath);
	}

	void EditorLayer::SaveSceneAs()
	{
		if (m_State != EditorState::EDIT)
			return;

		std::filesystem::path path = FileDialogs::SaveFile("Eis Scene (.eis)\0*.eis\0");

		if (path.empty())
			return;

		path = std::filesystem::relative(path, Project::GetAssetsDir());

		SceneImporter::SaveScene(m_EditedScene, path);
		const AssetHandle newSceneHandle = Project::GetEditorAssetManager()->ImportAsset(path);
		OpenScene(newSceneHandle);
	}


	void EditorLayer::OpenScene()
	{
		if (m_State != EditorState::EDIT)
			return;

		std::filesystem::path path = FileDialogs::OpenFile("Eis Scene (.eis)\0*.eis\0");

		if (path.empty())
			return;

		path = std::filesystem::relative(path, Project::GetAssetsDir());

		AssetHandle sceneHandle = Project::GetEditorAssetManager()->ImportAsset(path);
		OpenScene(sceneHandle);
	}

	void EditorLayer::OpenScene(AssetHandle handle)
	{
		EIS_CORE_ASSERT(AssetManager::IsHandleValid(handle));

		if (m_State != EditorState::EDIT)
			return;
		if (AssetManager::GetAssetType(handle) != AssetType::Scene)
			return;

		Ref<Scene> readOnlyScene = AssetManager::GetAsset<Scene>(handle);
		if (readOnlyScene)
		{
			m_EditedScene = Scene::Copy(readOnlyScene);
			m_EditedScene->OnViewportResize(m_ViewportSize.x, m_ViewportSize.y);
			m_HierarchyPanel->SetScene(m_EditedScene);

			m_EditedScenePath = Project::GetEditorAssetManager()->GetFilePath(handle);

			m_ActiveScene = m_EditedScene;

			// TODO: reset editor camera
		}
	}


	void EditorLayer::ScenePlay()
	{
		m_State = EditorState::PLAY;
		
		m_ActiveScene = Scene::Copy(m_EditedScene);
		m_ActiveScene->OnStartRuntime();

		m_HierarchyPanel->SetScene(m_ActiveScene);
	}

	void EditorLayer::SceneStop()
	{
		m_State = EditorState::EDIT;

		m_ActiveScene = m_EditedScene;
		m_ActiveScene->OnEndRuntime();

		m_HierarchyPanel->SetScene(m_ActiveScene);
	}



	void EditorLayer::OnEvent(Event& event)
	{
		m_EditorCam.OnEvent(event);

		EventDispatcher d{ event };
		d.Dispatch<MouseScrolledEvent>([](MouseScrolledEvent& e) { EIS_INFO("{}", e.GetYOffset()); return false; });
		d.Dispatch<KeyPressedEvent>(EIS_BIND_EVENT_FN(OnKeyPressed));
		d.Dispatch<MouseButtonPressedEvent>(EIS_BIND_EVENT_FN(OnMouseButtonPressed));
	}

	bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
	{
		if (e.GetRepeatCount() > 0)
			return false;

		const bool shift = Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift);
		const bool ctrl = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl);

		switch (e.GetKeyCode())
		{
			// Scene
			case Key::N:
				if (ctrl && shift)
					NewScene();
				break;

			case Key::O:
				if (ctrl)
					OpenScene();
				break;

			case Key::S:
				if (ctrl)
				{
					if (!shift)
						SaveScene();
					else
						SaveSceneAs();
				}
				break;

			// Tools
			case Key::Q:
				if (!ImGuizmo::IsUsing())
					m_GizmoType = -1;
				break;

			case Key::W:
				if (!ImGuizmo::IsUsing())
					m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
				break;

			case Key::E:
				if (!ImGuizmo::IsUsing())
					m_GizmoType = ImGuizmo::OPERATION::ROTATE;
				break;

			case Key::R:
				if (!ImGuizmo::IsUsing())
					m_GizmoType = ImGuizmo::OPERATION::SCALE;
				break;
		}

		return false;
	}

	bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		switch (e.GetMouseButton())
		{
			case Mouse::ButtonLeft:
				// Entity selection
				if (m_MousePosInViewport.x >= 0 && m_MousePosInViewport.y >= 0
					&& m_MousePosInViewport.x < (int)m_ViewportSize.x && m_MousePosInViewport.y < (int)m_ViewportSize.y
					&& m_ViewportHovered
					&& (!ImGuizmo::IsOver() || !m_HierarchyPanel->GetSelectedEntity()) // IsOver is buggy, not enough alone
					&& !Input::IsKeyPressed(Key::LeftShift))
				{
					const int pixelData = m_Framebuffer->ReadPixel(1, m_MousePosInViewport.x, m_MousePosInViewport.y);
					if (pixelData != -1)
					{
						Entity selected{ (entt::entity)pixelData, m_ActiveScene.get() };
						m_HierarchyPanel->SetSelectedEntity(selected);
					}
					else
						m_HierarchyPanel->SetSelectedEntity();
				}
				break;
		}

		return false;
	}
}