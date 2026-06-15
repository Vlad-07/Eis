#pragma once

#include "Eis/Core/Log.h"
#include "Eis/Core/UUID.h"

#include <string>


namespace Eis
{
	using AssetHandle = UUID;

	enum class AssetType : uint16_t
	{
		None = 0,
		Scene,
		Texture2D,
		Shader,
		Material
	};

	std::string AssetTypeToString(AssetType type);
	AssetType AssetTypeFromString(std::string_view str);


	class Asset
	{
	public:
		AssetHandle Handle{ 0 };

		virtual AssetType GetType() const = 0;
	};
}