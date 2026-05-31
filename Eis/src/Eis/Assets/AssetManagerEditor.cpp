#include "Eispch.h"
#include "AssetManagerEditor.h"
#include "AssetImporter.h"
#include "Eis/Project/Project.h"

#include <json.hpp>


namespace Eis
{
	static std::map<std::filesystem::path, AssetType> s_AssetExtensionMap
	{
		{ ".eis", AssetType::Scene},
		{ ".png", AssetType::Texture2D },
		{ ".jpg", AssetType::Texture2D },
		{ ".jpeg", AssetType::Texture2D },
		{ ".glsl", AssetType::Shader }
	};

	static AssetType AssetTypeFromExtension(const std::filesystem::path& extension)
	{
		auto it = s_AssetExtensionMap.find(extension);
		if (it == s_AssetExtensionMap.end())
		{
			EIS_CORE_ERROR("Unknown extension: {}", extension.string());
			return AssetType::None;
		}

		return it->second;
	}


	bool AssetManagerEditor::IsHandleValid(AssetHandle handle) const
	{
		return handle != 0 && m_AssetRegistry.contains(handle);
	}

	bool AssetManagerEditor::IsAssetLoaded(AssetHandle handle) const
	{
		return m_LoadedAssets.contains(handle);
	}


	const AssetMetadata& AssetManagerEditor::GetMetadata(AssetHandle handle) const
	{
		static AssetMetadata nullMetadata{};

		auto it = m_AssetRegistry.find(handle);
		if (it == m_AssetRegistry.end())
			return nullMetadata;

		return it->second;
	}

	const std::filesystem::path& AssetManagerEditor::GetFilePath(AssetHandle handle) const
	{
		return GetMetadata(handle).FilePath;
	}


	Ref<Asset> AssetManagerEditor::GetAsset(AssetHandle handle)
	{
		if (!IsHandleValid(handle))
		{
			EIS_CORE_ERROR("Invalid handle!");
			return nullptr;
		}

		if (IsAssetLoaded(handle))
		{
			return m_LoadedAssets.at(handle);
		}
		else // Load asset
		{
			const AssetMetadata& metadata = GetMetadata(handle);

			Ref<Asset> asset;
			asset = AssetImporter::ImportAsset(handle, metadata);
			if (!asset)
			{
				EIS_CORE_ERROR("Failed to import asset!");
				return nullptr;
			}

			m_LoadedAssets[handle] = asset;

			return asset;
		}
	}

	AssetType AssetManagerEditor::GetAssetType(AssetHandle handle) const
	{
		auto it = m_AssetRegistry.find(handle);
		if (it == m_AssetRegistry.end())
			return AssetType::None;

		return it->second.Type;
	}

	AssetHandle AssetManagerEditor::GetAssetByPath(const std::filesystem::path& path) const
	{
		for (const auto& [handle, metadata] : m_AssetRegistry)
		{
			if (metadata.FilePath == path)
				return handle;
		}

		return 0;
	}


	AssetHandle AssetManagerEditor::ImportAsset(const std::filesystem::path& path)
	{
		AssetHandle handle = GetAssetByPath(path);
		if (handle) return handle; // Already imported

		handle = AssetHandle{}; // Generate a new handle
		AssetMetadata metadata;
		metadata.Type = AssetTypeFromExtension(path.extension());
		metadata.FilePath = path;

		Ref<Asset> asset = AssetImporter::ImportAsset(handle, metadata);
		if (!asset)
		{
			EIS_CORE_ERROR("Failed to import asset!");
			return 0;
		}

		asset->Handle = handle;

		m_AssetRegistry[handle] = std::move(metadata);
		m_LoadedAssets[handle] = std::move(asset);

		SerializeAssetRegistry();

		return handle;
	}



	using json = nlohmann::ordered_json;

	void AssetManagerEditor::SerializeAssetRegistry()
	{
		const auto& path = Project::GetAssetRegistryPath();
		
		auto reg = json::array();
		for (const auto& [handle, metadata] : m_AssetRegistry)
		{
			auto obj = json::object();
			obj["Handle"] = (uint64_t)handle;
			obj["FilePath"] = metadata.FilePath.generic_string();
			obj["Type"] = AssetTypeToString(metadata.Type);

			reg.push_back(obj);
		}
		
		std::ofstream out{ path };
		out << reg.dump(4);
	}

	bool AssetManagerEditor::DeserializeAssetRegistry()
	{
		const auto& path = Project::GetAssetRegistryPath();

		if (!std::filesystem::exists(path))
		{
			EIS_CORE_ERROR("Could not find asset registry file: {}!", path.string());
			return false;
		}

		std::ifstream in{ path };
		auto reg = json::parse(in);
		for (const auto& obj : reg)
		{
			const AssetHandle handle = obj["Handle"].get<uint64_t>();

			AssetMetadata& metadata = m_AssetRegistry[handle];
			metadata.Type = AssetTypeFromString(obj["Type"].get<std::string>());
			metadata.FilePath = obj["FilePath"].get<std::string>();
		}

		return true;
	}
}