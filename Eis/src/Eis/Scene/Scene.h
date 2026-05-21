#pragma once

#include <entt.hpp>

#include "Eis/Core/UUID.h"


namespace Eis
{
	class Entity;
	class EditorCamera;

	class Scene
	{
	public:
		Scene();
		~Scene() = default;

		static Ref<Scene> Copy(const Ref<Scene>& other);


		Entity CreateEntity(std::string_view name, UUID uuid = UUID{});
		void DestroyEntity(Entity entity);


		void OnUpdateEditor(EditorCamera& camera);

		void OnStartRuntime();
		void OnUpdateRuntime();
		void OnEndRuntime();

		void OnViewportResize(uint32_t width, uint32_t height);

		std::string_view GetName() const { return m_Name; }

	private:
		void OnCameraComponentConstruct(entt::registry& registry, entt::entity entityId);

	private:
		std::string m_Name{ "Untitled" };

		entt::registry m_Registry{};

		uint32_t m_ViewportWidth{}, m_ViewportHeight{};

		friend class Entity;
		friend class HierarchyPanel;
		friend class SceneSerializer;
	};
}