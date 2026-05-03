#pragma once

#include <string>
#include <glm/glm.hpp>

#include "Eis/Rendering/Objects/Camera.h"


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
		glm::mat4 Transform{};

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::mat4& tr) : Transform{ tr } {}

		operator glm::mat4() const { return Transform; }
	};

	struct CameraComponent
	{
		Camera Camera;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
		CameraComponent(const glm::mat4& projection) : Camera{ projection } {}

		operator Eis::Camera() const { return Camera; }
	};
}