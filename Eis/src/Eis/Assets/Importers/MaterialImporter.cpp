#include "Eispch.h"
#include "MaterialImporter.h"

#include "Eis/Project/Project.h"

// temporary
#include <json/json.hpp>


namespace Eis
{
	Ref<MaterialBase> MaterialImporter::ImportMaterial(AssetHandle handle, const AssetMetadata& metadata)
	{
		return LoadMaterial(Project::GetAssetDir() / metadata.FilePath);
	}

	Ref<MaterialBase> MaterialImporter::LoadMaterial(const std::filesystem::path& path)
	{
		if (!std::filesystem::exists(path))
		{
			EIS_CORE_ERROR("Could not find material file: {}", path.string());
			return nullptr;
		}

		std::string type;
		{
			std::ifstream in{ path };
			nlohmann::json j = nlohmann::json::parse(in);

			type = j["Material"].get<std::string>();
		}

		if (type == "DefaultPBR")
		{
			Ref<MaterialBase> material = CreateRef<DefaultPBRMaterial>();
			material->Deserialize(path);
			return material;
		}

		EIS_CORE_ERROR("Unknown material type: {}", type);
		return nullptr;
	}
}