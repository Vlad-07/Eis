#pragma once

#include "Project.h"


namespace Eis
{
	class ProjectSerializer
	{
	public:
		ProjectSerializer(const Ref<Project>& proj)
			: m_Project{ proj } {}

		void Serialize(const std::filesystem::path& path);
		bool Deserialize(const std::filesystem::path& path);

	private:
		Ref<Project> m_Project;
	};
}