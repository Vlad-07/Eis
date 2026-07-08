#pragma once

#include "Shader.h"


namespace Eis
{
	// Compiles shader sources to SPIR-V for shaders to transpile in platform specific formats

	class ShaderCompiler
	{
	public:
		static ShaderBinaries Compile(const std::string& sources, std::string_view name, const std::vector<std::string>& defines = {});

	private:
		static ShaderSources PreProcess(const std::string& source);
		static ShaderBinaries CompileToSPV(const std::string& source, std::string_view name, const std::vector<std::string>& defines);
	};
}