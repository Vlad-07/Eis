#include "Eispch.h"
#include "Asset.h"


namespace Eis
{
	std::string_view AssetTypeToString(AssetType type)
	{
		switch (type)
		{
			case AssetType::None: return "None";
			case AssetType::Scene: return "Scene";
			case AssetType::Texture2D: return "Texture2D";
		}

		return "InvalidType";
	}

	AssetType AssetTypeFromString(std::string_view str)
	{
		if (str == "Scene") return AssetType::Scene;
		if (str == "Texture2D") return AssetType::Texture2D;
		if (str == "None") return AssetType::None;

		return AssetType::None;
	}
}