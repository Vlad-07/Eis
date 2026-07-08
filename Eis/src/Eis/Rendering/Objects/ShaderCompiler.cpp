#include "Eispch.h"
#include "ShaderCompiler.h"

#include <shaderc/shaderc.hpp>

#include <rapidhash/rapidhash.h>
#include <json/json.hpp>


using json = nlohmann::json;

namespace Eis
{
	namespace
	{
		shaderc_shader_kind GLShaderStageToShaderc(ShaderStage stage)
		{
			switch (stage)
			{
				case ShaderStage::Vertex: return shaderc_shader_kind::shaderc_vertex_shader;
				case ShaderStage::Fragment: return shaderc_shader_kind::shaderc_fragment_shader;
				default: EIS_CORE_ASSERT(false); return {};
			}
		}

		std::string StageCacheFileExtSPV(ShaderStage stage)
		{
			switch (stage)
			{
				case ShaderStage::Vertex: return ".cached_spv.vert";
				case ShaderStage::Fragment: return ".cached_spv.frag";
				default: EIS_CORE_ASSERT(false); return "";
			}
		}

		const std::filesystem::path& GetCacheDir()
		{
			static const std::filesystem::path cacheDir{ "resources/cache/shaders/opengl" };
			return cacheDir;
		}
		const std::filesystem::path& GetCacheRegistryPath()
		{
			static const std::filesystem::path cacheRegPath{ "resources/cache/shaders/opengl/ShaderCacheRegistry.shreg" };
			return cacheRegPath;
		}

		void CheckCache()
		{
			const std::filesystem::path& path = GetCacheDir();
			if (!std::filesystem::exists(path))
				std::filesystem::create_directories(path);

			const std::filesystem::path& regPath = GetCacheRegistryPath();
			if (!std::filesystem::exists(regPath))
			{
				std::ofstream out{ regPath };
				out << "{}";
			}
		}


		bool ReadBinary(std::vector<uint32_t>& data, const std::filesystem::path& path)
		{
			std::ifstream in{ path, std::ios::binary };

			if (!in.is_open())
				return false;

			in.seekg(0, std::ios::end);
			size_t size = in.tellg();
			in.seekg(0, std::ios::beg);

			data.resize(size / sizeof(uint32_t));
			in.read((char*)data.data(), size);

			return true;
		}
		void WriteBinary(const std::vector<uint32_t>& data, const std::filesystem::path& path)
		{
			std::ofstream out{ path, std::ios::binary };

			if (!out.is_open())
				return;

			out.write((const char*)data.data(), data.size() * sizeof(uint32_t));
		}
	}



    ShaderBinaries Eis::ShaderCompiler::Compile(const std::string& source, std::string_view name, const std::vector<std::string>& defines)
    {
		CheckCache();

		// TODO: hash defines

		const uint64_t hash = rapidhash(source.data(), source.size());

		bool upToDate{ false };
		std::ifstream in{ GetCacheRegistryPath() };
		json j = json::parse(in);
		in.close();

		if (j.contains(name))
		{
			const uint64_t oldHash = j[name]["Hash"].get<uint64_t>();
			upToDate = (oldHash == hash);
		}

		ShaderBinaries SPVBinaries;
		if (!upToDate)
		{
			// Compile
			SPVBinaries = CompileToSPV(source, name, defines);

			// Write SPIR-V
			for (const auto& [stage, binary] : SPVBinaries)
				WriteBinary(binary, GetCacheDir() / (name.data() + StageCacheFileExtSPV(stage)));

			// Write registry
			j[name]["Hash"] = hash;
			for (const auto& [stage, unused] : SPVBinaries)
				j[name]["Stages"].push_back(ShaderStageToString(stage));

			std::ofstream out{ GetCacheRegistryPath() };
			out << j.dump(4);
		}
		else
		{
			// Load SPIR-V
			for (const auto& s : j[name]["Stages"])
			{
				ShaderStage stage = ShaderStageFromString(s.get<std::string>());

				auto& binary = SPVBinaries[stage];
				ReadBinary(binary, GetCacheDir() / (name.data() + StageCacheFileExtSPV(stage)));
			}
		}

		return SPVBinaries;
    }


	ShaderSources ShaderCompiler::PreProcess(const std::string& source)
	{
		ShaderSources sources;

		const std::string typeToken{ "//type" };
		
		// Start of shader type declaration line
		size_t pos = source.find(typeToken, 0);
		while (pos != std::string::npos)
		{
			// End of shader type declaration line
			const size_t eol = source.find_first_of("\r\n", pos);
			EIS_CORE_ASSERT(eol != std::string::npos, "Shader source must have CRLF line endings!");

			// Start of shader type name (after "//type" keyword)
			const size_t begin = pos + typeToken.length() + 1;

			const std::string type = source.substr(begin, eol - begin);
			const ShaderStage stage = ShaderStageFromString(type);


			// Start of shader code after shader type declaration line
			const size_t nextLinePos = source.find_first_not_of("\r\n", eol);

			pos = source.find(typeToken, nextLinePos); // Start of next shader type declaration line

			sources[stage] = (pos == std::string::npos) ? source.substr(nextLinePos) : source.substr(nextLinePos, pos - nextLinePos);
		}

		return sources;
	}


	ShaderBinaries ShaderCompiler::CompileToSPV(const std::string& source, std::string_view name, const std::vector<std::string>& defines)
	{
		const ShaderSources sources = PreProcess(source);

		shaderc::Compiler compiler;
		shaderc::CompileOptions options;
		// vk 1.3+ breaks discard
		options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_2);
		options.SetOptimizationLevel(shaderc_optimization_level_zero);

		for (const std::string& d : defines)
			options.AddMacroDefinition(d);

		ShaderBinaries SPVBinaries;
		for (const auto& [stage, src] : sources)
		{
			shaderc::SpvCompilationResult module
				= compiler.CompileGlslToSpv(src, GLShaderStageToShaderc(stage), name.data(), options);
			if (module.GetCompilationStatus() != shaderc_compilation_status_success)
			{
				EIS_CORE_ERROR("GLSL to SPIR-V compilation failed!");
				EIS_CORE_ERROR("{}", module.GetErrorMessage());
				EIS_CORE_ASSERT(false);
			}

			SPVBinaries[stage].assign(module.cbegin(), module.cend());
		}

		return SPVBinaries;
	}
}