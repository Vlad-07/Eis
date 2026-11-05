#pragma once

#include "Eis/Core/Core.h"
#include "Eis/Assets/Asset.h"
#include "Eis/Rendering/Objects/Shader.h"
#include "Eis/Rendering/Objects/Buffers.h"

#include <vector>


namespace Eis
{
	// Stupid hardcoded materials for each shader (really stupid and temporary)

	class MaterialBase : public Asset
	{
	public:
		virtual ~MaterialBase() = default;

		void Bind();

		virtual void Serialize(const std::filesystem::path& path) = 0;
		virtual bool Deserialize(const std::filesystem::path& path) = 0;


		const Ref<Shader>& GetShader() const { return m_Shader; }

		static AssetType GetStaticType() { return AssetType::Material; }
		virtual AssetType GetType() const override { return GetStaticType(); }

	protected:
		virtual void UploadData() = 0;

	protected:
		Ref<Shader> m_Shader;

		Ref<UniformBuffer> m_MaterialBuffer;
		uint8_t m_MaterialBufferBinding{};
	};

	class DefaultPBRMaterial : public MaterialBase
	{
	public:
		DefaultPBRMaterial();

		void SetDiffuseMap(AssetHandle diffuseMap);

		virtual void Serialize(const std::filesystem::path& path) override;
		virtual bool Deserialize(const std::filesystem::path& path) override;

	private:
		virtual void UploadData() override;

	private:
		AssetHandle m_DiffuseMap;
		uint8_t m_DiffuseMapBinding{};

		struct MaterialData
		{
			;
		} m_MaterialData{};
	};

	/* This is set aside for now...
	using MaterialParameterValue = std::variant<
		bool,
		int32_t,
		float,
		glm::vec2,
		glm::vec3,
		glm::vec4,
		glm::mat4>;

	struct MaterialParameter
	{
		std::string Name;
		MaterialParameterValue Value;
	};


	class Material : public Asset
	{
	public:
		Material() = default;
		Material(AssetHandle shaderHandle) { SetShader(shaderHandle); }
		Material(const Ref<Shader>& shader) { SetShader(shader); }


		const Ref<Shader>& GetShader() const { return m_Shader; }
		void SetShader(AssetHandle shaderHandle);
		void SetShader(const Ref<Shader>& shader);

		void Bind() const;

		void SetInt(const std::string& name, int value);
		void SetFloat(const std::string& name, float value);
		void SetFloat2(const std::string& name, glm::vec2 value);
		void SetFloat3(const std::string& name, glm::vec3 value);
		void SetFloat4(const std::string& name, glm::vec4 value);
		void SetMat4(const std::string& name, const glm::mat4& value);




	private:
		void SetParameter(const std::string& name, BaseDataType dataType, uint8_t vecSize, uint8_t columns, const void* value, size_t size);

		Ref<Shader> m_Shader;
		Ref<UniformBuffer> m_MaterialBuffer;

		uint8_t m_MaterialBufferBinding{};
		std::vector<uint8_t> m_MaterialData;

		friend class MaterialSerializer;
	};


	class MaterialSerializer
	{
	public:
		MaterialSerializer(const Ref<Material>& material)
			: m_Material{ material } {}

		void Serialize(const std::filesystem::path& path);
		void SerializeRuntime(const std::filesystem::path& path);

		bool Deserialize(const std::filesystem::path& path);
		bool DeserializeRuntime(const std::filesystem::path& path);

	private:
		Ref<Material> m_Material;
	};//*/
}