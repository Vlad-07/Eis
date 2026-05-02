#include "Eispch.h"
#include "Scene.h"
#include "Entity.h"
#include "Components.h"


namespace Eis
{
	Scene::Scene()
	{
	}

	Scene::~Scene()
	{
	}

	Entity Scene::CreateEntity(std::string_view name)
	{
		Entity e{ m_Registry.create(), this };
		e.AddComponent<TransformComponent>();
		(e.AddComponent<TagCompontent>()).Tag = !name.empty() ? name : "Entity";

		return e;
	}
}