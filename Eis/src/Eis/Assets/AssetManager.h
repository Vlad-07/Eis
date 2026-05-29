#pragma once

#include "AssetManagerBase.h"
#include "Eis/Project/Project.h"


namespace Eis
{
	// Static interface
	class AssetManager
	{
	public:
		template<typename T>
		static Ref<T> GetAsset(AssetHandle handle)
		{
			return std::static_pointer_cast<T>(Project::GetAssetManager()->GetAsset(handle));
		}

		static AssetType GetAssetType(AssetHandle handle)
		{
			return Project::GetAssetManager()->GetAssetType(handle);
		}

		static bool IsHandleValid(AssetHandle handle)
		{
			return Project::GetAssetManager()->IsHandleValid(handle);
		}

		static bool IsAssetLoaded(AssetHandle handle)
		{
			return Project::GetAssetManager()->IsAssetLoaded(handle);
		}
	};
}