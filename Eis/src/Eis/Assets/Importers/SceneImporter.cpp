#include "Eispch.h"
#include "SceneImporter.h"

#include "Eis/Project/Project.h"

#include "Eis/Scene/SceneSerializer.h"



namespace Eis
{
	Ref<Scene> SceneImporter::ImportScene(AssetHandle handle, const AssetMetadata& metadata)
	{
		Ref<Scene> scene = CreateRef<Scene>();
		SceneSerializer s{ scene };
		if (!s.Deserialize(Project::GetAssetsDir() / metadata.FilePath))
		{
			EIS_CORE_ERROR("Scene deserialization failed!");
			return nullptr;
		}

		return scene;
	}

	void SceneImporter::SaveScene(const Ref<Scene>& scene, const std::filesystem::path& path)
	{
		SceneSerializer s{ scene };
		s.Serialize(Project::GetAssetsDir() / path);
	}
}