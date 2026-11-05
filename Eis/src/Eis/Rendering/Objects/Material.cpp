#include "Eispch.h"
#include "Material.h"

#include "Eis/Assets/AssetManager.h"

#include "Eis/Rendering/Objects/Texture.h"

#include "Eis/Utils/JsonUtils.h"


using json = nlohmann::ordered_json;

namespace Eis
{
	void MaterialBase::Bind()
	{
		if (!m_Shader) // TODO: error shader
			return;

		m_Shader->Bind();
		UploadData();
		if (m_MaterialBuffer)
			m_MaterialBuffer->Bind(m_MaterialBufferBinding);
	}




	DefaultPBRMaterial::DefaultPBRMaterial()
	{
		AssetHandle shader{ Project::GetEditorAssetManager()->ImportAsset(Project::GetAssetDir() / "Shaders" / "DefaultPBR.glsl") };
		m_Shader = AssetManager::GetAsset<Shader>(shader);

		if (!m_Shader)
			return;

		for (const UniformBufferBlock& block : m_Shader->GetReflection().UniformBuffers)
		{
			if (block.Name == "Material")
			{
				m_MaterialBufferBinding = block.Binding;
				m_MaterialBuffer = UniformBuffer::Create(block.BlockSize, block.Binding);
				return;
			}
		}

		m_DiffuseMapBinding = m_Shader->GetReflection().Samplers[0].Binding;
	}


	void DefaultPBRMaterial::SetDiffuseMap(AssetHandle diffuseMap)
	{
		m_DiffuseMap = diffuseMap;
	}

	void DefaultPBRMaterial::UploadData()
	{
		AssetManager::GetAsset<Texture2D>(m_DiffuseMap)->Bind(m_DiffuseMapBinding);

		if (m_MaterialBuffer)
			m_MaterialBuffer->SetData(&m_MaterialData, sizeof(MaterialData));
	}

	void DefaultPBRMaterial::Serialize(const std::filesystem::path& path)
	{
		json j;
		j["Material"] = "DefaultPBR";
		j["Shader"] = (uint64_t)m_Shader->Handle;

		j["Parameters"] = json::object();
		j["Parameters"]["DiffuseMap"] = (uint64_t)m_DiffuseMap;

		std::ofstream fout{ path };
		fout << j.dump(4);
	}

	bool DefaultPBRMaterial::Deserialize(const std::filesystem::path& path)
	{
		if (!std::filesystem::exists(path))
		{
			EIS_CORE_ERROR("Could not find material file: {}", path.string());
			return false;
		}

		std::ifstream fin{ path };
		const json j = json::parse(fin);

		if (!j.contains("Material"))
		{
			EIS_CORE_ERROR("Invalid material file: {}!", path.string());
			return false;
		}

		if (j["Material"].get<std::string>() != "DefaultPBR")
		{
			EIS_CORE_ERROR("Invalid material type!");
			return false;
		}

		m_Shader = AssetManager::GetAsset<Shader>(j["Shader"].get<uint64_t>());

		m_DiffuseMap = j["Parameters"]["DiffuseMap"].get<uint64_t>();

		return true;
	}

	/*
	void Material::SetShader(AssetHandle shaderHandle)
	{
        SetShader(AssetManager::GetAsset<Shader>(shaderHandle));
	}

	void Material::SetShader(const Ref<Shader>& shader)
	{
		m_Shader = shader;
		m_MaterialBuffer.reset();
		m_MaterialData.clear();

		if (!m_Shader)
			return;

		for (const UniformBufferBlock& block : m_Shader->GetReflection().UniformBuffers)
		{
			if (block.Name == "Material")
			{
				m_MaterialBufferBinding = block.Binding;
				m_MaterialData.resize(block.BlockSize, 0);
				if (!m_MaterialData.empty())
				{
					m_MaterialBuffer = UniformBuffer::Create(block.BlockSize, block.Binding);
					m_MaterialBuffer->SetData(m_MaterialData.data(), m_MaterialData.size());
				}
				return;
			}
		}
	}

	void Material::Bind() const
	{
		if (!m_Shader) // TODO: error shader
			return;

		m_Shader->Bind();
		if (m_MaterialBuffer)
			m_MaterialBuffer->Bind(m_MaterialBufferBinding);
	}

	void Material::SetInt(const std::string& name, int value)
	{
		SetParameter(name, BaseDataType::Int, 1, 1, &value, sizeof(value));
	}

	void Material::SetFloat(const std::string& name, float value)
	{
		SetParameter(name, BaseDataType::Float, 1, 1, &value, sizeof(value));
	}

	void Material::SetFloat2(const std::string& name, glm::vec2 value)
	{
		SetParameter(name, BaseDataType::Float, 2, 1, &value, sizeof(value));
	}

	void Material::SetFloat3(const std::string& name, glm::vec3 value)
	{
		SetParameter(name, BaseDataType::Float, 3, 1, &value, sizeof(value));
	}

	void Material::SetFloat4(const std::string& name, glm::vec4 value)
	{
		SetParameter(name, BaseDataType::Float, 4, 1, &value, sizeof(value));
	}

	void Material::SetMat4(const std::string& name, const glm::mat4& value)
	{
		SetParameter(name, BaseDataType::Float, 4, 4, &value, sizeof(value));
	}

	void Material::SetParameter(const std::string& name, BaseDataType dataType, uint8_t vecSize, uint8_t columns, const void* value, size_t size)
	{
		if (!m_Shader || !m_MaterialBuffer)
			return;

		for (const UniformBufferBlock& block : m_Shader->GetReflection().UniformBuffers)
		{
			if (block.Name == "Material")
			{
				for (const UniformBufferMember& member : block.Members)
				{
					if (member.Name == name)
					{
						const bool isIntBool = dataType == BaseDataType::Int && member.DataType == BaseDataType::Bool;
						if ((member.DataType != dataType && !isIntBool) || member.VecSize != vecSize || member.Columns != columns || size > member.ByteSize)
						{
							EIS_CORE_ASSERT(false, "Material parameter does not match shader reflection: {}", name);
							return;
						}

						std::memcpy(m_MaterialData.data() + member.ByteOffset, value, size);
						m_MaterialBuffer->SetData(m_MaterialData.data() + member.ByteOffset, size, member.ByteOffset);
						return;
					}
				}
			}
		}
	}



	void MaterialSerializer::Serialize(const std::filesystem::path& path)
	{
		EIS_CORE_ASSERT(m_Material);

		json j;
		j["Material"] = ""; // maybe put some data here?
		j["Shader"] = static_cast<uint64_t>(m_Material->GetShader()->Handle);

		j["Parameters"] = json::array();

		if (!m_Material->m_MaterialBuffer)
			return;

		for (const UniformBufferBlock& block : m_Material->GetShader()->GetReflection().UniformBuffers)
		{
			if (block.Binding == m_Material->m_MaterialBufferBinding)
			{
				for (const auto& member : block.Members)
					j["Parameters"][member.Name]; // write member
			}
		}
	}//*/
}