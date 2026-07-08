#include "Eispch.h"
#include "ShaderImporter.h"

#include "Eis/Project/Project.h"
#include "Eis/Utils/FileUtils.h"


namespace Eis
{
	Ref<Shader> ShaderImporter::ImportShader(AssetHandle handle, const AssetMetadata& metadata)
	{
		return LoadShader(Project::GetAssetDir() / metadata.FilePath);
	}

	Ref<Shader> ShaderImporter::LoadShader(const std::filesystem::path& path)
	{
		const std::string name = path.filename().string();
		const std::string source = Utils::ReadFile(path);

		return Shader::Create(name, source);
	}
}