#include "Eispch.h"
#include "AssetImporter.h"

#include "Importers/SceneImporter.h"
#include "Importers/TextureImporter.h"
#include "Importers/ShaderImporter.h"
#include "Importers/MeshImporter.h"


namespace Eis
{
	using AssetLoadingFn = std::function<Ref<Asset>(AssetHandle, const AssetMetadata&)>;
	static std::map<AssetType, AssetLoadingFn> s_AssetLoadingFns
	{
		{ AssetType::Scene, SceneImporter::ImportScene },
		{ AssetType::Texture2D, TextureImporter::ImportTexture2D },
		{ AssetType::Shader, ShaderImporter::ImportShader },
		//{ AssetType::Material, MaterialImporter::ImportMaterial },
		{ AssetType::Mesh, MeshImporter::ImportMesh }
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