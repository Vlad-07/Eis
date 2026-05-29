#pragma once

#include "Eis/Core/Core.h"

#include "Asset.h"
#include "AssetMetadata.h"


namespace Eis
{
	class AssetImporter
	{
	public:
		static Ref<Asset> ImportAsset(AssetHandle handle, const AssetMetadata& metadata);
	};
}