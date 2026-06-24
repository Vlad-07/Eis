#include "Eispch.h"
#include "ProjectSerializer.h"

#include <fstream>
#include <json/json.hpp>


namespace Eis
{
	using json = nlohmann::ordered_json;


	void ProjectSerializer::Serialize(const std::filesystem::path& path)
	{
		EIS_CORE_ASSERT(m_Project);

		const ProjectConfig& config = m_Project->GetConfig();

		json j;
		auto& proj = j["Project"] = j.object();

		proj["AssetDir"] = config.AssetsDirectory.string();
		proj["AssetReg"] = config.AssetRegistryPath.string();
		proj["StartScene"] = (uint64_t)config.StartingScene;

		std::ofstream out{ path };
		out << j.dump(4);
	}

	bool ProjectSerializer::Deserialize(const std::filesystem::path& path)
	{
		EIS_CORE_ASSERT(m_Project);

		if (!std::filesystem::exists(path))
		{
			EIS_CORE_ERROR("Project file not found: {}!", path.string());
			return false;
		}

		ProjectConfig& config = m_Project->GetConfig();

		std::ifstream in{ path };
		auto j = json::parse(in);
		if (!j.contains("Project"))
		{
			EIS_CORE_ERROR("Invalid project file: {}!", path.string());
			return false;
		}

		auto& proj = j["Project"];

		config.Name = path.stem().string();
		config.AssetsDirectory = proj["AssetDir"].get<std::filesystem::path>();
		config.AssetRegistryPath = proj["AssetReg"].get<std::filesystem::path>();
		config.StartingScene = proj["StartScene"].get<uint64_t>();

		return true;
	}
}