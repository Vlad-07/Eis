#include "Eispch.h"
#include "Scene.h"
#include "Entity.h"
#include "Components.h"

#include "Eis/Assets/AssetManager.h"
#include "Eis/Scene/ScriptableEntity.h"
#include "Eis/Rendering/Renderer/Renderer2D.h"
#include "Eis/Rendering/Objects/EditorCamera.h"


namespace Eis
{
	Ref<Scene> Scene::Copy(const Ref<Scene>& other)
	{
		Ref<Scene> newScene = CreateRef<Scene>();

		newScene->m_Name = other->m_Name;
		newScene->m_ViewportWidth = other->m_ViewportWidth;
		newScene->m_ViewportHeight = other->m_ViewportHeight;

		// might be faster to iterate per component type
		// or entt::snapshot
		other->m_Registry.view<IDComponent, TagCompontent, TransformComponent>().each(
		[&](const entt::entity entityId, IDComponent& id,
			TagCompontent& tag, TransformComponent& tr)
		{
			Entity original = Entity{ entityId, other.get() };
			Entity newEnt = newScene->CreateEntity(tag.Tag, id.ID);

			newEnt.AddOrReplaceComponent<TransformComponent>(original.GetComponent<TransformComponent>());

			if (original.HasComponent<SpriteRendererComponent>())
				newEnt.AddOrReplaceComponent<SpriteRendererComponent>(original.GetComponent<SpriteRendererComponent>());

			if (original.HasComponent<CameraComponent>())
				newEnt.AddOrReplaceComponent<CameraComponent>(original.GetComponent<CameraComponent>());

			if (original.HasComponent<NativeScriptComponent>())
				newEnt.AddOrReplaceComponent<NativeScriptComponent>(original.GetComponent<NativeScriptComponent>());
		});

		return newScene;
	}



	Scene::Scene()
	{
		m_Registry.on_construct<CameraComponent>().connect<&Scene::OnCameraComponentConstruct>(this);
	}


	Entity Scene::CreateEntity(std::string_view name, UUID uuid)
	{
		Entity e{ m_Registry.create(), this };
		e.AddComponent<IDComponent>(uuid);
		e.AddComponent<TransformComponent>();
		(e.AddComponent<TagCompontent>()).Tag = !name.empty() ? name : "Entity";

		return e;
	}

	void Scene::DestroyEntity(Entity entity)
	{
		m_Registry.destroy(entity);
	}


	void Scene::OnStartRuntime()
	{
		m_Registry.view<NativeScriptComponent>().each([](auto entity, NativeScriptComponent& nsc)
		{
			if (!nsc.Instance)
			{
				nsc.InstantiateScript();
				nsc.Instance->OnCreate();
			}
		});
	}

	void Scene::OnUpdateRuntime()
	{
		// Scripts
		m_Registry.view<NativeScriptComponent>().each([](auto entity, NativeScriptComponent& nsc)
		{
			nsc.Instance->OnUpdate();
		});

		// TODO: active camera system
		Camera* mainCamera{};
		glm::mat4 cameraTransform{};
		auto view = m_Registry.view<TransformComponent, CameraComponent>();
		if (view.front() != entt::null)
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
				auto [trc, src] = group.get<TransformComponent, SpriteRendererComponent>(entity);

				const glm::mat4 tr = trc.GetTransform();
				if (src.Texture != 0)
				{
					Ref<Texture2D> tex = AssetManager::GetAsset<Texture2D>(src.Texture);
					Renderer2D::DrawQuad(tr, tex, src.Tint, 1.0f, (int32_t)entity);
				}
				else
				{
					Renderer2D::DrawQuad(tr, src.Tint, (int32_t)entity);
				}
			}

			Renderer2D::EndScene();
		}
	}

	void Scene::OnEndRuntime()
	{
		m_Registry.view<NativeScriptComponent>().each([](auto entity, NativeScriptComponent& nsc)
		{
			nsc.Instance->OnDestroy();
			nsc.DestroyScript();
		});
	}



	void Scene::OnUpdateEditor(EditorCamera& camera)
	{
		Renderer2D::BeginScene(camera);

		auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		for (auto entity : group)
		{
			auto [trc, src] = group.get<TransformComponent, SpriteRendererComponent>(entity);

			const glm::mat4 tr = trc.GetTransform();
			if (src.Texture != 0)
			{
				Ref<Texture2D> tex = AssetManager::GetAsset<Texture2D>(src.Texture);
				Renderer2D::DrawQuad(tr, tex, src.Tint, 1.0f, (int32_t)entity);
			}
			else
			{
				Renderer2D::DrawQuad(tr, src.Tint, (int32_t)entity);
			}
		}

		Renderer2D::EndScene();
	}


	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		m_Registry.view<CameraComponent>().each([=](auto entity, CameraComponent& cameraComponent)
		{
			if (!cameraComponent.FixedAspectRatio)
				cameraComponent.Camera.SetViewportSize(width, height);
		});
	}



	void Scene::OnCameraComponentConstruct(entt::registry& registry, entt::entity entityId)
	{
		CameraComponent& component = registry.get<CameraComponent>(entityId);

		if (!component.FixedAspectRatio)
			component.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
	}
}