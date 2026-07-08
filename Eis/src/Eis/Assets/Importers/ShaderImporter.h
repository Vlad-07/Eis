#pragma once

#include "Eis/Core/Core.h"

#include "Eis/Assets/Asset.h"
#include "Eis/Assets/AssetMetadata.h"

#include "Eis/Rendering/Objects/Shader.h"


namespace Eis
{
	class ShaderImporter
	{
	public:
		static Ref<Shader> ImportShader(AssetHandle handle, const AssetMetadata& metadata);

		static Ref<Shader> LoadShader(const std::filesystem::path& path);
	};
}