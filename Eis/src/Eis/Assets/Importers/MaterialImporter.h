#pragma once

#include "Eis/Core/Core.h"

#include "Eis/Assets/Asset.h"
#include "Eis/Assets/AssetMetadata.h"

#include "Eis/Rendering/Objects/Material.h"


namespace Eis
{
	class MaterialImporter
	{
	public:
		static Ref<MaterialBase> ImportMaterial(AssetHandle handle, const AssetMetadata& metadata);

		static Ref<MaterialBase> LoadMaterial(const std::filesystem::path& path);
	};
}