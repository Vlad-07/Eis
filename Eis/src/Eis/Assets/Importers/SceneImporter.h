#pragma once

#include "Eis/Core/Core.h"

#include "Eis/Assets/Asset.h"
#include "Eis/Assets/AssetMetadata.h"

#include "Eis/Scene/Scene.h"


namespace Eis
{
	class SceneImporter
	{
	public:
		static Ref<Scene> ImportScene(AssetHandle handle, const AssetMetadata& metadata);

		static void SaveScene(const Ref<Scene>& scene, const std::filesystem::path& path);
	};
}