#include "Eispch.h"
#include "OpenGLShader.h"

#include "Eis/Project/Project.h"
#include "Eis/Rendering/Objects/ShaderReflector.h"

#include <ios>
#include <fstream>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include <shaderc/shaderc.hpp>
#include <spirv_cross.hpp>
#include <spirv_glsl.hpp>

#include <rapidhash/rapidhash.h>
#include <json/json.hpp>


using json = nlohmann::json;

namespace Eis
{
	namespace
	{
		GLenum ShaderStageToGL(ShaderStage stage)
		{
			switch (stage)
			{
				case Eis::ShaderStage::Vertex: return GL_VERTEX_SHADER;
				case Eis::ShaderStage::Fragment: return GL_FRAGMENT_SHADER;
				default: EIS_CORE_ASSERT(false); return 0;
			}
		}

		shaderc_shader_kind GLShaderStageToShaderc(ShaderStage stage)
		{
			switch (stage)
			{
				case ShaderStage::Vertex: return shaderc_shader_kind::shaderc_vertex_shader;
				case ShaderStage::Fragment: return shaderc_shader_kind::shaderc_fragment_shader;
				default: EIS_CORE_ASSERT(false); return {};
			}
		}

		std::string ShaderStageCacheFileExtVK(ShaderStage stage)
		{
			switch (stage)
			{
				case ShaderStage::Vertex: return ".cached_vk.vert";
				case ShaderStage::Fragment: return ".cached_vk.frag";
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


	OpenGLShader::OpenGLShader(std::string_view name, const std::string& source)
		: m_Name{ name }
	{
		EIS_PROFILE_RENDERER_FUNCTION();

		CheckCache();

		const uint64_t hash = rapidhash(source.data(), source.size());

		bool upToDate{ false };
		std::ifstream in{ GetCacheRegistryPath() };
		json j = json::parse(in);
		in.close();

		if (j.contains(m_Name))
		{
			const uint64_t oldHash = j[m_Name]["Hash"].get<uint64_t>();
			upToDate = (oldHash == hash);
		}

		// TODO: take this registry garbage out of the shader class

		if (!upToDate)
		{
			// Compile
			ShaderSources shaderSources = PreProcess(source);
			m_VKBinaries = CompileToVK(shaderSources, m_Name);
			m_GLSLsources = CompileToGLSL(m_VKBinaries);
			UploadSources(m_GLSLsources);

			// Write KV
			for (const auto& [stage, binary] : m_VKBinaries)
				WriteBinary(binary, GetCacheDir() / (m_Name + ShaderStageCacheFileExtVK(stage)));

			// Write registry
			j[m_Name]["Hash"] = hash;
			for (const auto& [stage, unused] : m_VKBinaries)
				j[m_Name]["Stages"].push_back(ShaderStageToString(stage));

			std::ofstream out{ GetCacheRegistryPath() };
			out << j.dump(4);
		}
		else
		{
			// Load VK
			for (const auto& s : j[m_Name]["Stages"])
			{
				ShaderStage stage = ShaderStageFromString(s.get<std::string>());

				auto& binary = m_VKBinaries[stage];
				ReadBinary(binary, GetCacheDir() / (m_Name + ShaderStageCacheFileExtVK(stage)));
			}

			// cahce intermediate glsl?
			m_GLSLsources = CompileToGLSL(m_VKBinaries);
			UploadSources(m_GLSLsources);
		}

		// TODO: cache reflection
		m_Reflection = ShaderReflector::Reflect(m_VKBinaries);
	}

	OpenGLShader::~OpenGLShader()
	{
		EIS_PROFILE_RENDERER_FUNCTION();

		glDeleteProgram(m_RendererId);
	}


	ShaderSources OpenGLShader::PreProcess(const std::string& source)
	{
		EIS_PROFILE_RENDERER_FUNCTION();

		ShaderSources shaderSources;

		const std::string typeToken{ "//type" };
		size_t pos = source.find(typeToken, 0); // Start of shader type declaration line
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

			shaderSources[stage] = (pos == std::string::npos) ? source.substr(nextLinePos) : source.substr(nextLinePos, pos - nextLinePos);
		}

		return shaderSources;
	}

	ShaderBinaries OpenGLShader::CompileToVK(const ShaderSources& glslSources, std::string_view name)
	{
		shaderc::Compiler compiler;
		shaderc::CompileOptions options;
		// vk 1.3+ breaks discard
		options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_2);
		options.SetOptimizationLevel(shaderc_optimization_level_zero);

		ShaderBinaries output;
		for (const auto& [stage, src] : glslSources)
		{
			shaderc::SpvCompilationResult module
				= compiler.CompileGlslToSpv(src, GLShaderStageToShaderc(stage), name.data(), options);
			if (module.GetCompilationStatus() != shaderc_compilation_status_success)
			{
				EIS_CORE_ERROR("GLSL to SPIR-V compilation failed!");
				EIS_CORE_ERROR("{}", module.GetErrorMessage());
				EIS_CORE_ASSERT(false);
			}

			std::vector<uint32_t>& stageData = output[stage];
			stageData.assign(module.cbegin(), module.cend());
		}

		return output;
	}

	ShaderSources OpenGLShader::CompileToGLSL(const ShaderBinaries& vkBinaries)
	{
		ShaderSources output;
		for (const auto& [stage, binary] : vkBinaries)
		{
			std::string& glslSrc = output[stage];
			spirv_cross::CompilerGLSL glslCompiler{ binary };
			glslSrc = glslCompiler.compile();
		}
		return output;
	}


	void OpenGLShader::UploadSources(const ShaderSources& shaderSources)
	{
		EIS_PROFILE_RENDERER_FUNCTION();

		const GLuint program = glCreateProgram();

		// TODO: consider  stack array instead of a heap vector for performance reasons
		std::vector<GLuint> glShaderIDs;
		glShaderIDs.reserve(shaderSources.size());
		for (const auto& [stage, src] : shaderSources)
		{
			const GLuint shader = glShaderIDs.emplace_back(glCreateShader(ShaderStageToGL(stage)));

			const GLchar* sourceCStr = src.c_str();
			glShaderSource(shader, 1, &sourceCStr, 0);

			glCompileShader(shader);

			GLint isCompiled{};
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLen{};
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLen);

				std::vector<GLchar> infoLog(maxLen);
				glGetShaderInfoLog(shader, maxLen, &maxLen, infoLog.data());

				glDeleteShader(shader);

				EIS_CORE_ERROR("{}", infoLog.data());
				EIS_CORE_ASSERT(false, "Shader compilation failure!");
				break;
			}

			glAttachShader(program, shader);
		}

		glLinkProgram(program);

		GLint isLinked{};
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLen{};
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLen);

			std::vector<GLchar> infoLog(maxLen);
			glGetProgramInfoLog(program, maxLen, &maxLen, &infoLog[0]);

			glDeleteProgram(program);

			for (auto id : glShaderIDs)
				glDeleteShader(id);

			EIS_CORE_ERROR("{}", infoLog.data());
			EIS_CORE_ASSERT(false, "Shader link failure!");
			return;
		}

		for (auto id : glShaderIDs)
		{
			glDetachShader(program, id);
			glDeleteShader(id);
		}

		m_RendererId = program;
	}


	void OpenGLShader::Bind() const
	{
		EIS_PROFILE_RENDERER_FUNCTION();

		glUseProgram(m_RendererId);
	}

	void OpenGLShader::Unbind() const
	{
		EIS_PROFILE_RENDERER_FUNCTION();

		glUseProgram(0);
	}

	// TODO: maybe location caching

	void OpenGLShader::SetInt(const std::string& name, int value)
	{
		EIS_PROFILE_RENDERER_FUNCTION();

		const int32_t location = glGetUniformLocation(m_RendererId, name.c_str());
		glUniform1i(location, value);
	}

	void OpenGLShader::SetIntArray(const std::string& name, const int* values, uint32_t count)
	{
		EIS_PROFILE_RENDERER_FUNCTION();

		const int32_t location = glGetUniformLocation(m_RendererId, name.c_str());
		glUniform1iv(location, count, values);
	}

	void OpenGLShader::SetFloat(const std::string& name, float value)
	{
		EIS_PROFILE_RENDERER_FUNCTION();

		const int32_t location = glGetUniformLocation(m_RendererId, name.c_str());
		glUniform1f(location, value);
	}

	void OpenGLShader::SetFloat2(const std::string& name, glm::vec2 value)
	{
		EIS_PROFILE_RENDERER_FUNCTION();

		const int32_t location = glGetUniformLocation(m_RendererId, name.c_str());
		glUniform2f(location, value.x, value.y);
	}

	void OpenGLShader::SetFloat3(const std::string& name, glm::vec3 value)
	{
		EIS_PROFILE_RENDERER_FUNCTION();

		const int32_t location = glGetUniformLocation(m_RendererId, name.c_str());
		glUniform3f(location, value.x, value.y, value.z);
	}

	void OpenGLShader::SetFloat4(const std::string& name, glm::vec4 value)
	{
		EIS_PROFILE_RENDERER_FUNCTION();

		const int32_t location = glGetUniformLocation(m_RendererId, name.c_str());
		glUniform4f(location, value.x, value.y, value.z, value.w);
	}

	void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& value)
	{
		EIS_PROFILE_RENDERER_FUNCTION();

		const int32_t location = glGetUniformLocation(m_RendererId, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}
}