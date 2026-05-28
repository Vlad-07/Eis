#pragma once

#include "Eis/Debug/Assert.h"

#include "Eis/Assets/Asset.h"
#include "Eis/Assets/AssetManagerEditor.h"


namespace Eis
{
	struct ProjectConfig
	{
		std::string Name{ "Untitled" };

		std::filesystem::path AssetsDirectory{ "Assets" };
		std::filesystem::path AssetRegistryPath{ "Assets/AssetRegistry.ereg" };

		AssetHandle StartingScene{ 0 };
	};


	class Project
	{
	public:
		static Ref<Project> GetActive() { return s_ActiveProject; }

		static const std::filesystem::path& GetProjectPath()
		{
			EIS_CORE_ASSERT(s_ActiveProject);
			return s_ActiveProject->m_ProjectPath;
		}

		static std::filesystem::path GetAssetsDir()
		{
			EIS_CORE_ASSERT(s_ActiveProject);
			return GetProjectPath() / s_ActiveProject->m_Config.AssetsDirectory;
		}

		static std::filesystem::path GetAssetRegistryPath()
		{
			EIS_CORE_ASSERT(s_ActiveProject);
			return GetProjectPath() / s_ActiveProject->m_Config.AssetRegistryPath;
		}


		static std::shared_ptr<AssetManagerBase> GetAssetManager()
		{
			EIS_CORE_ASSERT(s_ActiveProject);
			return s_ActiveProject->m_AssetManager;
		}
		static std::shared_ptr<AssetManagerEditor> GetEditorAssetManager()
		{
			EIS_CORE_ASSERT(s_ActiveProject);
			return std::static_pointer_cast<AssetManagerEditor>(s_ActiveProject->m_AssetManager);
		}


		ProjectConfig& GetConfig() { return m_Config; }


		static Ref<Project> New();
		static Ref<Project> Load(const std::filesystem::path& path);
		static void SaveActive(const std::filesystem::path& path);

	private:
		ProjectConfig m_Config;

		std::filesystem::path m_ProjectPath;

		std::shared_ptr<AssetManagerBase> m_AssetManager;


		static inline Ref<Project> s_ActiveProject;
	};
}