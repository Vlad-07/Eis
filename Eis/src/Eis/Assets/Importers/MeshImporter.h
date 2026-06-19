#pragma once

#include "Eis/Core/Core.h"

#include "Eis/Assets/Asset.h"
#include "Eis/Assets/AssetMetadata.h"

#include "Eis/Rendering/Objects/Mesh.h"


namespace Eis
{
	class MeshImporter
	{
	public:
		static Ref<Mesh> ImportMesh(AssetHandle handle, const AssetMetadata& metadata);

		static Ref<Mesh> LoadMesh(const std::filesystem::path& path);
	};
}