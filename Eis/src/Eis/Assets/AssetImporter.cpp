#include "Eispch.h"
#include "AssetImporter.h"

#include "Importers.h"


namespace Eis
{
	using AssetLoadingFn = std::function < Ref<Asset>(AssetHandle, const AssetMetadata&)>;
	static std::map<AssetType, AssetLoadingFn> s_AssetLoadingFns
	{
		{ AssetType::Scene, SceneImporter::ImportScene },
		{ AssetType::Texture2D, TextureImporter::ImportTexture2D }
	};

	Ref<Asset> AssetImporter::ImportAsset(AssetHandle handle, const AssetMetadata& metadata)
	{
		auto it = s_AssetLoadingFns.find(metadata.Type);
		if (it == s_AssetLoadingFns.end())
		{
			EIS_CORE_ERROR("No importer available for asset type: {}!", (uint16_t)metadata.Type);
			return nullptr;
		}

		return it->second(handle, metadata);
	}
}