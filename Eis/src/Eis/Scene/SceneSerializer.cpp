#include "Eispch.h"
#include "SceneSerializer.h"
#include "Entity.h"
#include "Components.h"

#include <json.hpp>


using json = nlohmann::ordered_json;

namespace glm
{
	static void to_json(json& j, vec2 v)
	{
		j = { v.x, v.y };
	}
	static void from_json(const json& j, vec2& v)
	{
		v.x = j[0]; v.y = j[1];
	}


	static void to_json(json& j, vec3 v)
	{
		j = { v.x, v.y, v.z };
	}
	static void from_json(const json& j, vec3& v)
	{
		v.x = j[0]; v.y = j[1]; v.z = j[2];
	}


	static void to_json(json& j, vec4 v)
	{
		j = { v.x, v.y, v.z, v.w };
	}
	static void from_json(const json& j, vec4& v)
	{
		v.x = j[0]; v.y = j[1]; v.z = j[2]; v.w = j[3];
	}
}

namespace Eis
{
	static void to_json(json& j, Entity entity)
	{
		j["Entity"] = static_cast<uint64_t>(entity.GetUUID());
		
		if (entity.HasComponent<TagCompontent>())
		{
			const auto& component = entity.GetComponent<TagCompontent>();

			j["TagComponent"] = json::object();
			j["TagComponent"]["Tag"] = component.Tag;
		}

		if (entity.HasComponent<TransformComponent>())
		{
			auto& component = entity.GetComponent<TransformComponent>();

			j["TransformComponent"] = json::object();
			j["TransformComponent"]["Translation"] = component.Translation;
			j["TransformComponent"]["Rotation"] = component.Rotation;
			j["TransformComponent"]["Scale"] = component.Scale;
		}

		if (entity.HasComponent<CameraComponent>())
		{
			auto& component = entity.GetComponent<CameraComponent>();

			j["CameraComponent"] = json::object();
			
			j["CameraComponent"]["Camera"] = json::object();
			j["CameraComponent"]["Camera"]["ProjectionType"] = component.Camera.GetProjectionType();
			j["CameraComponent"]["Camera"]["PerspectiveFOV"] = component.Camera.GetPerspectiveFov();
			j["CameraComponent"]["Camera"]["PerspectiveNear"] = component.Camera.GetPerspectiveNear();
			j["CameraComponent"]["Camera"]["PerspectiveFar"] = component.Camera.GetPerspectiveFar();
			j["CameraComponent"]["Camera"]["OrthoSize"] = component.Camera.GetOrthoSize();
			j["CameraComponent"]["Camera"]["OrthoNear"] = component.Camera.GetOrthoNear();
			j["CameraComponent"]["Camera"]["OrthoFar"] = component.Camera.GetOrthoFar();

			j["CameraComponent"]["FixedAspect"] = component.FixedAspectRatio;
		}

		if (entity.HasComponent<SpriteRendererComponent>())
		{
			auto& component = entity.GetComponent<SpriteRendererComponent>();

			j["SpriteRendererComponent"] = json::object();
			j["SpriteRendererComponent"]["Tint"] = component.Tint;
		}
	}


	SceneSerializer::SceneSerializer(const Ref<Scene>& scene)
		: m_Scene{ scene } {}

	void SceneSerializer::Serialize(const std::filesystem::path& path)
	{
		m_Scene->m_Name = path.stem().string();

		json j;
		j["Scene"] = ""; // maybe put some data here?
		j["Entities"] = json::array();
		m_Scene->m_Registry.view<entt::entity>().each([&](auto entityId)
		{
			Entity entity{ entityId, m_Scene.get() };
			j["Entities"].push_back(entity);
		});

		std::ofstream fout{ path };
		fout << j.dump(4);
	}


	void SceneSerializer::SerializeRuntime(const std::filesystem::path& path)
	{
		EIS_CORE_ASSERT(false, "SceneSerializer::SerializeRuntime not implemented!");
	}



	bool SceneSerializer::Deserialize(const std::filesystem::path& path)
	{
		if (!std::filesystem::exists(path))
		{
			EIS_CORE_ERROR("Could not find scene: '{}'", path.string());
			return false;
		}

		std::ifstream fin{ path };
		const json j = json::parse(fin);

		if (!j.contains("Scene"))
		{
			EIS_CORE_ERROR("Invalid scene: {}!", path.string());
			return false;
		}

		m_Scene->m_Name = path.stem().string();
		EIS_CORE_TRACE("Deserializing scene '{}'", m_Scene->m_Name);

		for (const auto& entity : j["Entities"])
		{
			const UUID uuid = entity["Entity"].get<uint64_t>();
			const std::string name = entity["TagComponent"]["Tag"].get<std::string>();

			//EIS_CORE_TRACE("Deserialized entity '{}', Id:{}", name, (uint64_t)uuid);

			Entity deserializedEntity = m_Scene->CreateEntity(name, uuid);

			if (entity.contains("TransformComponent"))
			{
				const auto& component = entity["TransformComponent"];
				auto& tc = deserializedEntity.GetComponent<TransformComponent>();

				tc.Translation = component["Translation"].get<glm::vec3>();
				tc.Rotation = component["Rotation"].get<glm::vec3>();
				tc.Scale = component["Scale"].get<glm::vec3>();
			}

			if (entity.contains("CameraComponent"))
			{
				const auto& component = entity["CameraComponent"];

				auto& cc = deserializedEntity.AddComponent<CameraComponent>();


				cc.Camera.SetProjectionType((SceneCamera::ProjectionType)component["Camera"]["ProjectionType"].get<int>());

				cc.Camera.SetPerspectiveFov(component["Camera"]["PerspectiveFOV"].get<float>());
				cc.Camera.SetPerspectiveNear(component["Camera"]["PerspectiveNear"].get<float>());
				cc.Camera.SetPerspectiveFar(component["Camera"]["PerspectiveFar"].get<float>());

				cc.Camera.SetOrthoSize(component["Camera"]["OrthoSize"].get<float>());
				cc.Camera.SetOrthoNear(component["Camera"]["OrthoNear"].get<float>());
				cc.Camera.SetOrthoFar(component["Camera"]["OrthoFar"].get<float>());
			}

			if (entity.contains("SpriteRendererComponent"))
			{
				const auto& component = entity["SpriteRendererComponent"];

				auto& src = deserializedEntity.AddComponent<SpriteRendererComponent>();


				src.Tint = component["Tint"].get<glm::vec4>();
			}
		}

		return true;
	}


	bool SceneSerializer::DeserializeRuntime(const std::filesystem::path& path)
	{
		EIS_CORE_ASSERT(false, "SceneSerializer::DeserializeRuntime not implemented!");
		return false;
	}
}