#pragma once

#include "Shader.h"


namespace Eis
{
	class ShaderReflector
	{
	public:
		static ShaderReflection Reflect(const ShaderBinaries& binaries);
	};
}