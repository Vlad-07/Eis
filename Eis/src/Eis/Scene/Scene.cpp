#include "Eispch.h"
#include "Scene.h"
#include "Entity.h"
#include "Components.h"

#include "Eis/Rendering/Renderer/Renderer2D.h"


#include "Eis/Rendering/Objects/OrthoCameraController.h"


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

	void Scene::DestroyEntity(Entity entity)
	{
		m_Registry.destroy(entity);
	}


	void Scene::OnUpdate()
	{
		// Scripts
		m_Registry.view<NativeScriptComponent>().each([](auto entity, NativeScriptComponent& nsc) {
			if (!nsc.Instance)
			{
				nsc.InstantiateScript();
				nsc.Instance->OnCreate();
			}

			nsc.Instance->OnUpdate();
		});

		// TODO: active camera system
		Camera* mainCamera{};
		glm::mat4 cameraTransform{};
		auto view = m_Registry.view<TransformComponent, CameraComponent>();
		for (auto entity : view.each())
		{
			mainCamera = &view.get<CameraComponent>(view.front()).Camera;
			cameraTransform = view.get<TransformComponent>(view.front()).GetTransform();
		}

		// Rendering
		if (mainCamera)
		{
			Renderer2D::BeginScene(*mainCamera, cameraTransform);

			auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (auto entity : group)
			{
				auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

				Renderer2D::DrawQuad(transform.GetTransform(), sprite.Tint);
			}

			Renderer2D::EndScene();
		}
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		m_Registry.view<CameraComponent>().each([=](auto entity, CameraComponent& cameraComponent) {
			if (!cameraComponent.FixedAspectRatio)
				cameraComponent.Camera.SetViewportSize(width, height);
		});
	}
}