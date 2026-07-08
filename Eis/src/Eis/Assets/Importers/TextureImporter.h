#pragma once

#include "Eis/Core/Core.h"

#include "Eis/Assets/Asset.h"
#include "Eis/Assets/AssetMetadata.h"

#include "Eis/Rendering/Objects/Texture.h"


namespace Eis
{
	class TextureImporter
	{
	public:
		static Ref<Texture2D> ImportTexture2D(AssetHandle handle, const AssetMetadata& metadata);

		static Ref<Texture2D> LoadTexture2D(const std::filesystem::path& path);
	};
}