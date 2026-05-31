#pragma once

#include "Eis/Core/Core.h"

#include "Asset.h"
#include "AssetMetadata.h"

#include "Eis/Scene/Scene.h"
#include "Eis/Rendering/Objects/Texture.h"
#include "Eis/Rendering/Objects/Shader.h"


namespace Eis
{
	class SceneImporter
	{
	public:
		static Ref<Scene> ImportScene(AssetHandle handle, const AssetMetadata& metadata);

		static void SaveScene(const Ref<Scene>& scene, const std::filesystem::path& path);
	};

	class TextureImporter
	{
	public:
		static Ref<Texture2D> ImportTexture2D(AssetHandle handle, const AssetMetadata& metadata);

		// Used for non asset textures (eg. editor resources)
		static Ref<Texture2D> LoadTexture2D(const std::filesystem::path& path);
	};

	class ShaderImporter
	{
	public:
		static Ref<Shader> ImportShader(AssetHandle handle, const AssetMetadata& metadata);

		// Used for non asset shaders (eg. editor resources)
		static Ref<Shader> LoadShader(const std::filesystem::path& path);
	};
}