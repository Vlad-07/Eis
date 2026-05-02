#pragma once

#include <string>
#include <glm/glm.hpp>


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
}