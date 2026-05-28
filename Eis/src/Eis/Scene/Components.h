#pragma once

#include "Eis/Core/UUID.h"
#include "Eis/Assets/Asset.h"
#include "SceneCamera.h"

#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>


namespace Eis
{
	struct IDComponent
	{
		UUID ID{};

		IDComponent() = default;
		IDComponent(const IDComponent&) = default;
		IDComponent(UUID uuid) : ID{ uuid } {}
	};

	struct TagCompontent
	{
		std::string Tag{};

		TagCompontent() = default;
		TagCompontent(const TagCompontent&) = default;
		TagCompontent(const std::string& tag) : Tag{ tag } {}
		TagCompontent(std::string_view tag) : Tag{ tag } {}
	};

	struct TransformComponent
	{
		glm::vec3 Translation{};
		glm::vec3 Rotation{};
		glm::vec3 Scale{ 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3& tr) : Translation{ tr } {}

		glm::mat4 GetTransform() const
		{
			const glm::mat4 rotation = glm::toMat4(glm::quat{ Rotation });

			return glm::translate(glm::mat4{1.0f}, Translation)
				* rotation
				* glm::scale(glm::mat4{1.0f}, Scale);
		}
	};


	struct SpriteRendererComponent
	{
		AssetHandle Texture{ 0 };
		glm::vec4 Tint{ 1.0f };

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
	};

	struct CameraComponent
	{
		SceneCamera Camera;
		bool FixedAspectRatio{};

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};


	class ScriptableEntity;

	struct NativeScriptComponent
	{
		ScriptableEntity* Instance{};
		std::function<void()> InstantiateScript;
		std::function<void()> DestroyScript;

		NativeScriptComponent() = default;
		NativeScriptComponent(const NativeScriptComponent& other)
			: InstantiateScript{ other.InstantiateScript }, DestroyScript{ other.DestroyScript }
		{}

		template<typename T>
		void Bind()
		{
			InstantiateScript = [&]() { Instance = new T{}; };
			DestroyScript = [&]() { delete Instance; Instance = nullptr; };
		}
	};
}