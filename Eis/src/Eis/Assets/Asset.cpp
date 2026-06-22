#include "Eispch.h"
#include "Asset.h"


namespace Eis
{
	std::string AssetTypeToString(AssetType type)
	{
		switch (type)
		{
			case AssetType::None:      return "None";
			case AssetType::Scene:     return "Scene";
			case AssetType::Texture2D: return "Texture2D";
			case AssetType::Shader:    return "Shader";
			case AssetType::Material:  return "Material";
			case AssetType::Mesh:  return "Mesh";
		}
		EIS_CORE_ERROR("Unknown AssetType: {}", (uint16_t)type);
		return "InvalidType";
	}

	AssetType AssetTypeFromString(std::string_view str)
	{
		if (str == "None")      return AssetType::None;
		if (str == "Scene")     return AssetType::Scene;
		if (str == "Texture2D") return AssetType::Texture2D;
		if (str == "Shader")    return AssetType::Shader;
		if (str == "Material")  return AssetType::Material;
		if (str == "Mesh")      return AssetType::Mesh;

		EIS_CORE_ERROR("Unknown AssetType string: {}", str);
		return AssetType::None;
	}
}