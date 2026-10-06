#include "Eispch.h"
#include "Material.h"

#include "Eis/Assets/AssetManager.h"


namespace Eis
{
	void Material::SetShader(AssetHandle shaderHandle)
	{
		m_Shader = AssetManager::GetAsset<Shader>(shaderHandle);
	}
}