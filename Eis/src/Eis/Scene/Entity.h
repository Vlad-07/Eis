#pragma once

#include <utility>
#include <entt.hpp>

#include "Eis/Debug/Assert.h"
#include "Scene.h"
#include "Components.h"


namespace Eis
{
	class Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity handle, Scene* scene) : m_EntityHandle{handle}, m_Scene{scene} {}
		Entity(const Entity&) = default;


		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			EIS_CORE_ASSERT(m_Scene, "Invalid entity!");
			EIS_CORE_ASSERT(!HasComponent<T>(), "Entity already has component!");

			return m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
		}

		template<typename T, typename... Args>
		T& AddOrReplaceComponent(Args&&... args)
		{
			EIS_CORE_ASSERT(m_Scene, "Invalid entity!");

			return m_Scene->m_Registry.emplace_or_replace<T>(m_EntityHandle, std::forward<Args>(args)...);
		}

		template<typename T>
		T& GetComponent()
		{
			EIS_CORE_ASSERT(m_Scene, "Invalid entity!");
			EIS_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");

			return m_Scene->m_Registry.get<T>(m_EntityHandle);
		}

		template<typename T>
		bool HasComponent()
		{
			EIS_CORE_ASSERT(m_Scene, "Invalid entity!");

			return m_Scene->m_Registry.all_of<T>(m_EntityHandle);
		}

		template<typename T>
		void RemoveComponent()
		{
			EIS_CORE_ASSERT(m_Scene, "Invalid entity!");
			EIS_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");

			m_Scene->m_Registry.remove<T>(m_EntityHandle);
		}


		UUID GetUUID() { return GetComponent<IDComponent>().ID; }
		std::string_view GetName() { return GetComponent<TagCompontent>().Tag; }


		operator bool() const { return m_Scene != nullptr ? m_Scene->m_Registry.valid(m_EntityHandle) : false; }
		operator uint32_t() const { return static_cast<uint32_t>(m_EntityHandle); }
		operator entt::entity() const { return m_EntityHandle; }

		bool operator==(Entity other) const
		{ return m_EntityHandle == other.m_EntityHandle && m_Scene == other.m_Scene; }

		bool operator!=(Entity other) const { return !(*this == other); }

	private:
		entt::entity m_EntityHandle{ entt::null };
		Scene* m_Scene{ nullptr };
	};
}