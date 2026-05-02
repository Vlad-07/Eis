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

	private:
		entt::registry m_Registry{};

		friend class Entity;
	};
}