#include "Eispch.h"
#include "Project.h"

#include "ProjectSerializer.h"


namespace Eis
{
	Ref<Project> Project::New()
	{
		s_ActiveProject = CreateRef<Project>();
		return s_ActiveProject;
	}

	Ref<Project> Project::Load(const std::filesystem::path& path)
	{
		Ref<Project> newProj = CreateRef<Project>();
		ProjectSerializer s{ newProj };
		if (s.Deserialize(path))
		{
			s_ActiveProject = newProj;
			s_ActiveProject->m_ProjectPath = path.parent_path();

			auto assetManager = std::make_shared<AssetManagerEditor>();
			s_ActiveProject->m_AssetManager = assetManager;
			assetManager->DeserializeAssetRegistry();

			return s_ActiveProject;
		}

		return nullptr;
	}

	void Project::SaveActive(const std::filesystem::path& path)
	{
		ProjectSerializer s{ s_ActiveProject };
		s.Serialize(path);
		s_ActiveProject->m_ProjectPath = path.parent_path();
	}
}