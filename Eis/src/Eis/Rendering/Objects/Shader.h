#pragma once

#include <string>
#include <unordered_map>
#include <filesystem>

#include <glm/glm.hpp>


namespace Eis
{
	class Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetInt(const std::string& name, int value) = 0;
		virtual void SetIntArray(const std::string& name, const int* values, uint32_t count) = 0;
		virtual void SetFloat(const std::string& name, float value) = 0;
		virtual void SetFloat2(const std::string& name, glm::vec2 value) = 0;
		virtual void SetFloat3(const std::string& name, glm::vec3 value) = 0;
		virtual void SetFloat4(const std::string& name, glm::vec4 value) = 0;
		virtual void SetMat4(const std::string& name, const glm::mat4& value) = 0;

		//virtual const std::string& GetName() const = 0;

		static Ref<Shader> Create(const std::filesystem::path& path);
//		static Ref<Shader> Create(const std::string& name, const std::string& vsSrc, const std::string& fsSrc);
	};
}