#pragma once

#include <entt.hpp>


namespace Eis
{
	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(std::string_view name);
		void DestroyEntity(Entity entity);

		void OnUpdate();

		void OnViewportResize(uint32_t width, uint32_t height);

	private:
		entt::registry m_Registry{};

		uint32_t m_ViewportWidth{}, m_ViewportHeight{};
		
		// proper api needed?
		friend class Entity;
		friend class HierarchyPanel;
	};
}