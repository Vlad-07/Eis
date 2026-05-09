#pragma once

#include "Entity.h"


namespace Eis
{
	class ScriptableEntity
	{
	public:
		virtual ~ScriptableEntity() = default;

		// Prototype

		virtual void OnCreate() {}
		virtual void OnDestroy() {}
		virtual void OnUpdate() {}

	protected:
		// Helpers

		template<typename T>
		T& GetComponent()
		{
			return m_Entity.GetComponent<T>();
		}

	private:
		Entity m_Entity;

		friend class Scene;
	};
}