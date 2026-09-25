#pragma once

#include "Eis/Core/Core.h"
#include "Eis/Assets/Asset.h"
#include "Eis/Rendering/Objects/Shader.h"


namespace Eis
{
	class Material : public Asset
	{
	public:
		Material() = default;
		Material(const Ref<Shader>& shader) : m_Shader{ shader } {}


		const Ref<Shader>& GetShader() const { return m_Shader; }


	private:
		Ref<Shader> m_Shader;
	};
}