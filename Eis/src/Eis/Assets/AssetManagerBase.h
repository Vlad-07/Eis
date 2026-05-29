#pragma once

#include "Eis/Core/Core.h"
#include "Asset.h"

#include <map>


namespace Eis
{
	using AssetMap = std::map<AssetHandle, Ref<Asset>>;

	class AssetManagerBase
	{
	public:
		virtual Ref<Asset> GetAsset(AssetHandle handle) = 0;
		virtual AssetType GetAssetType(AssetHandle handle) const = 0;

		virtual bool IsHandleValid(AssetHandle handle) const = 0;
		virtual bool IsAssetLoaded(AssetHandle handle) const = 0;

	protected:
		virtual ~AssetManagerBase() = 0 {}
	};
}