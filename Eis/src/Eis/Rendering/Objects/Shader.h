#pragma once

#include "Eis/Assets/Asset.h"
#include "ShaderData.h"

#include <string>
#include <glm/glm.hpp>


namespace Eis
{
	enum class ShaderStage : uint8_t
	{
		None = 0,

		Vertex,
		Fragment
	};

	ShaderStage ShaderStageFromString(const std::string& type);
	std::string ShaderStageToString(ShaderStage stage);


	// GLSL plain text
	using ShaderSources = std::unordered_map<ShaderStage, std::string>;
	// SPIR-V binary
	using ShaderBinaries = std::unordered_map<ShaderStage, std::vector<uint32_t>>;


	class Shader : public Asset
	{
	public:
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual const ShaderReflection& GetReflection() const = 0;
		virtual const AttributeLayout& GetAttributeLayout() const = 0;

		virtual void SetInt(const std::string& name, int value) = 0;
		virtual void SetIntArray(const std::string& name, const int* values, uint32_t count) = 0;
		virtual void SetFloat(const std::string& name, float value) = 0;
		virtual void SetFloat2(const std::string& name, glm::vec2 value) = 0;
		virtual void SetFloat3(const std::string& name, glm::vec3 value) = 0;
		virtual void SetFloat4(const std::string& name, glm::vec4 value) = 0;
		virtual void SetMat4(const std::string& name, const glm::mat4& value) = 0;

		//virtual const std::string& GetName() const = 0;

		static AssetType GetStaticType() { return AssetType::Shader; }
		virtual AssetType GetType() const override { return GetStaticType(); }


		static Ref<Shader> Create(std::string_view name, const std::string& source);
	};
}