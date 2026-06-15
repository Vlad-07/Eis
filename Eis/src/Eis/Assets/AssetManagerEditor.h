#pragma once

#include "AssetManagerBase.h"
#include "AssetMetadata.h"


namespace Eis
{
	using AssetRegistry = std::map<AssetHandle, AssetMetadata>;


	std::string AssetTypeToExtension(AssetType type);
	AssetType AssetTypeFromExtension(const std::string& ext);

	class AssetManagerEditor : public AssetManagerBase
	{
	public:
		virtual Ref<Asset> GetAsset(AssetHandle handle) override;
		virtual AssetType GetAssetType(AssetHandle handle) const override;

		virtual bool IsHandleValid(AssetHandle handle) const override;
		virtual bool IsAssetLoaded(AssetHandle handle) const override;


		// If path already belongs to an imported asset its handle is returned
		// Otherwise a new asset is created
		AssetHandle ImportAsset(const std::filesystem::path& path);


		const AssetMetadata& GetMetadata(AssetHandle handle) const;
		const std::filesystem::path& GetFilePath(AssetHandle handle) const;

		AssetHandle GetAssetByPath(const std::filesystem::path& path) const;

		const AssetRegistry& GetAssetRegistry() const { return m_AssetRegistry; }


		void SerializeAssetRegistry();
		bool DeserializeAssetRegistry();

	private:
		AssetRegistry m_AssetRegistry;
		AssetMap m_LoadedAssets;
	};
}