#include "Eispch.h"
#include "TextureImporter.h"

#include "Eis/Project/Project.h"

#include <stb_image.h>


namespace Eis
{
	Ref<Texture2D> TextureImporter::ImportTexture2D(AssetHandle handle, const AssetMetadata& metadata)
	{
		return LoadTexture2D(Project::GetAssetDir() / metadata.FilePath);
	}

	Ref<Texture2D> TextureImporter::LoadTexture2D(const std::filesystem::path& path)
	{
		stbi_set_flip_vertically_on_load(1);

		int width{}, height{}, channels{};
		Buffer data;
		{
			EIS_PROFILE_SCOPE("stbi_load - TextureImporter::ImportTexture2D");
			std::string pathStr = path.string();
			data.Data = stbi_load(pathStr.c_str(), &width, &height, &channels, 0);
		}

		if (!data)
		{
			EIS_CORE_ERROR("LoadTexture2D failed to load file: {}!", path.string());
			EIS_CORE_ERROR("stb_image: {}", stbi_failure_reason());
			return nullptr;
		}

		// stbi_load is 8-bits-per-channel only
		data.Size = static_cast<uint64_t>(width) * height * channels;

		TextureSpec texSpec;
		texSpec.Width = width;
		texSpec.Height = height;
		switch (channels)
		{
			case 3: texSpec.Format = ImageFormat::RGB8; break;
			case 4: texSpec.Format = ImageFormat::RGBA8; break;

			default: EIS_CORE_ASSERT(false); break;
		}

		Ref<Texture2D> texture = Texture2D::Create(texSpec, data);
		stbi_image_free(data.Data);
		return texture;
	}
}