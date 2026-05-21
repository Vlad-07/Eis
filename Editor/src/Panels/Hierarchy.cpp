#include "Hierarchy.h"
#include <imgui.h>
#include <imgui_internal.h>
#include <glm/gtc/type_ptr.hpp>
#include <Eis/Scene/Components.h>


namespace Eis
{
	HierarchyPanel::HierarchyPanel(const Ref<Scene>& scene)
	{
		SetScene(scene);
	}

	void HierarchyPanel::SetScene(const Ref<Scene>& scene)
	{
		m_Scene = scene;
		m_Selection = {};
	}

	void HierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Hierarchy");

		ImGuiTreeNodeFlags flags{};
		flags |= ImGuiTreeNodeFlags_Framed;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
		flags |= ImGuiTreeNodeFlags_DefaultOpen;
		flags |= ImGuiTreeNodeFlags_DrawLinesFull;
		if (ImGui::TreeNodeEx(m_Scene->GetName().data(), flags))
		{
			m_Scene->m_Registry.view<entt::entity>().each([&](entt::entity entityId)
			{
				Entity entity{ entityId, m_Scene.get() };
				DrawEntityNode(entity);
			});

			if (ImGui::IsWindowHovered() && ImGui::IsMouseDown(ImGuiMouseButton_Left))
				m_Selection = {};

			if (ImGui::BeginPopupContextWindow(0, ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems))
			{
				if (ImGui::MenuItem("Create Entity"))
					m_Scene->CreateEntity({});

				ImGui::EndPopup();
			}

			ImGui::TreePop();
		}
		ImGui::End();

		// should make its own class
		ImGui::Begin("Properties");

		if (m_Selection)
			DrawComponents(m_Selection);

		ImGui::End();
	}

	void HierarchyPanel::SetSelectedEntity(Entity entity)
	{
		m_Selection = entity;
	}

	void HierarchyPanel::DrawEntityNode(Entity entity)
	{
		auto& tag = entity.GetComponent<TagCompontent>();

		ImGuiTreeNodeFlags flags{};
		flags |= ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_OpenOnDoubleClick;
		flags |= ImGuiTreeNodeFlags_DrawLinesFull;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
		flags |= (m_Selection == entity ? ImGuiTreeNodeFlags_Selected : 0);
		flags |= ImGuiTreeNodeFlags_Leaf; // if no children // no parenting yet

		bool open = ImGui::TreeNodeEx((void*)(uintptr_t)(uint32_t)entity, flags, tag.Tag.c_str());

		if (ImGui::IsItemClicked())
			m_Selection = entity;

		bool deleted{};
		if (ImGui::BeginPopupContextItem(0, ImGuiPopupFlags_MouseButtonRight))
		{
			if (ImGui::MenuItem("Delete Entity"))
				deleted = true;

			ImGui::EndPopup();
		}

		if (open)
		{
			// draw children...

			ImGui::TreePop();
		}
		
		if (deleted)
		{
			m_Scene->DestroyEntity(entity);
			if (m_Selection == entity)
				m_Selection = {};
		}
	}





	static void DrawVec3Controls(std::string_view label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f)
	{
		ImGui::PushID(label.data());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.data());

		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, glm::vec2{});

		const float lineHeight = ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2.0f;
		const glm::vec2 buttonSize{ lineHeight + 3.0f, lineHeight };

		if (ImGui::Button("X", buttonSize))
			values.x = resetValue;
		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		if (ImGui::Button("Y", buttonSize))
			values.y = resetValue;
		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		if (ImGui::Button("Z", buttonSize))
			values.z = resetValue;
		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PopStyleVar();
		ImGui::Columns(1);

		ImGui::PopID();
	}


	template<typename T, typename UIFunc>
	static void DrawComponent(std::string_view name, Entity entity, UIFunc func)
	{
		auto& component = entity.GetComponent<T>();

		ImGuiTreeNodeFlags treeFlags{};
		treeFlags |= ImGuiTreeNodeFlags_Framed;
		treeFlags |= ImGuiTreeNodeFlags_FramePadding;
		treeFlags |= ImGuiTreeNodeFlags_DefaultOpen;
		treeFlags |= ImGuiTreeNodeFlags_AllowOverlap;
		treeFlags |= ImGuiTreeNodeFlags_SpanAvailWidth;

		float contentRegionAvail = ImGui::GetContentRegionAvail().x;

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, glm::vec2{ 4.0f, 4.0f });
		float lineHeight = ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2.0f;
		ImGui::Separator();
		bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeFlags, name.data());
		ImGui::PopStyleVar();
		ImGui::SameLine(contentRegionAvail - lineHeight * 0.5f);
		if (ImGui::Button("+", glm::vec2{ lineHeight }))
			ImGui::OpenPopup("ComponentSettings");

		bool removeComponent{};
		if (ImGui::BeginPopup("ComponentSettings"))
		{
			if (ImGui::MenuItem("Remove Component"))
				removeComponent = true;
			ImGui::EndPopup();
		}

		if (open)
		{
			func(component);

			ImGui::TreePop();
		}

		if (removeComponent)
			entity.RemoveComponent<T>();
	}


	void HierarchyPanel::DrawComponents(Entity entity)
	{
		if (entity.HasComponent<TagCompontent>())
		{
			// BUG: having the input text selected and clicking
			// on another entity in the hierarchy renames the clicked entity

			auto& tag = entity.GetComponent<TagCompontent>().Tag;
			UUID uuid = entity.GetComponent<IDComponent>().ID;

			char buff[256]{};
			strcpy_s(buff, sizeof(buff), tag.c_str());
			if (ImGui::InputText("##Tag", buff, sizeof(buff)))
				tag = std::string{ buff };

			ImGui::SetItemTooltip("ID: %llu", uuid);
		}

		ImGui::SameLine();
		ImGui::PushItemWidth(-1);
		if (ImGui::Button("Add Component"))
			ImGui::OpenPopup("AddComponent");

		if (ImGui::BeginPopup("AddComponent"))
		{
			if (!m_Selection.HasComponent<SpriteRendererComponent>())
			{
				if (ImGui::MenuItem("Sprite Renderer"))
				{
					m_Selection.AddComponent<SpriteRendererComponent>();
					ImGui::CloseCurrentPopup();
				}
			}

			if (!m_Selection.HasComponent<CameraComponent>())
			{
				if (ImGui::MenuItem("Camera"))
				{
					m_Selection.AddComponent<CameraComponent>();
					ImGui::CloseCurrentPopup();
				}
			}

			ImGui::EndPopup();
		}
		ImGui::PopItemWidth();


		if (entity.HasComponent<TransformComponent>())
		{
			DrawComponent<TransformComponent>("Transform", entity, [](auto& component)
			{
				DrawVec3Controls("Translation", component.Translation);

				glm::vec3 rot = glm::degrees(component.Rotation);
				DrawVec3Controls("Rotation", rot);
				component.Rotation = glm::radians(rot);

				DrawVec3Controls("Scale", component.Scale, 1.0f);
			});
		}

		if (entity.HasComponent<CameraComponent>())
		{
			DrawComponent<CameraComponent>("Camera", entity, [](auto& component)
			{
				SceneCamera& camera = component.Camera;

				const char* projectionTypeStrs[2]{ "Perspective", "Orthographic" };
				const char* currentProjTypeStr = projectionTypeStrs[static_cast<uint32_t>(camera.GetProjectionType())];

				if (ImGui::BeginCombo("Projection", currentProjTypeStr))
				{
					for (uint32_t i{}; i < 2; i++)
					{
						bool selected = currentProjTypeStr == projectionTypeStrs[i];
						if (ImGui::Selectable(projectionTypeStrs[i], selected))
						{
							currentProjTypeStr = projectionTypeStrs[i];
							camera.SetProjectionType(static_cast<SceneCamera::ProjectionType>(i));
						}

						if (selected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
				{
					float fov = glm::degrees(camera.GetPerspectiveFov());
					if (ImGui::DragFloat("Vertical FOV", &fov))
						camera.SetPerspectiveFov(glm::radians(fov));

					float nearClip = camera.GetPerspectiveNear();
					if (ImGui::DragFloat("Near", &nearClip))
						camera.SetPerspectiveNear(nearClip);

					float farClip = camera.GetPerspectiveFar();
					if (ImGui::DragFloat("Far", &farClip))
						camera.SetPerspectiveFar(farClip);
				}
				if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
				{
					float size = camera.GetOrthoSize();
					if (ImGui::DragFloat("Size", &size, 0.1f))
						camera.SetOrthoSize(size);

					float nearClip = camera.GetOrthoNear();
					if (ImGui::DragFloat("Near", &nearClip, 0.1f))
						camera.SetOrthoNear(nearClip);

					float farClip = camera.GetOrthoFar();
					if (ImGui::DragFloat("Far", &farClip, 0.1f))
						camera.SetOrthoFar(farClip);
				}
			});
		}

		if (entity.HasComponent<SpriteRendererComponent>())
		{
			DrawComponent<SpriteRendererComponent>("Sprite Renderer", entity, [](auto& component)
			{
				ImGui::ColorEdit4("Tint", glm::value_ptr(component.Tint));
			});
		}
	}
}