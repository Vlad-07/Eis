#pragma once

#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "SceneCamera.h"
#include "ScriptableEntity.h"


namespace Eis
{
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
			const glm::mat4 rotation = glm::rotate(glm::mat4{ 1.0f }, Rotation.x, glm::vec3{ 1.0f, 0.0f, 0.0f })
				* glm::rotate(glm::mat4{ 1.0f }, Rotation.y, glm::vec3{ 0.0f, 1.0f, 0.0f })
				* glm::rotate(glm::mat4{ 1.0f }, Rotation.z, glm::vec3{ 0.0f, 0.0f, 1.0f });

			return glm::translate(glm::mat4{1.0f}, Translation)
				* rotation
				* glm::scale(glm::mat4{1.0f}, Scale);
		}
	};

	struct SpriteRendererComponent
	{
		//...
		glm::vec4 Tint{ 1.0f };

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& tint) : Tint{ tint } {}
	};

	struct CameraComponent
	{
		SceneCamera Camera;
		bool FixedAspectRatio{};

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};

	struct NativeScriptComponent
	{
		template<typename T>
		void Bind()
		{
			InstantiateScript = [&]() { Instance = new T{}; };
			DestroyScript = [&]() { delete Instance; };
		}

	//private:
		ScriptableEntity* Instance;

		std::function<void()> InstantiateScript;
		std::function<void()> DestroyScript;
	};
}