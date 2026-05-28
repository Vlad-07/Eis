#pragma once

#include "Eis/Core/Core.h"

#include "Asset.h"
#include "AssetMetadata.h"

#include "Eis/Rendering/Objects/Texture.h"
#include "Eis/Scene/Scene.h"

namespace Eis
{
	namespace SceneImporter
	{
		Ref<Scene> ImportScene(AssetHandle handle, const AssetMetadata& metadata);

		void SaveScene(const Ref<Scene>& scene, const std::filesystem::path& path);
	}

	namespace TextureImporter
	{
		Ref<Texture2D> ImportTexture2D(AssetHandle handle, const AssetMetadata& metadata);

		// Used for non asset textures (eg. editor resources)
		Ref<Texture2D> LoadTexture2D(const std::filesystem::path& path);
	}
}