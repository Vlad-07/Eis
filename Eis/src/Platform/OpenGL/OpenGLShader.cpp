#include "Eispch.h"
#include "OpenGLShader.h"

#include "Eis/Project/Project.h"

#include <ios>
#include <fstream>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include <shaderc/shaderc.hpp>
#include <spirv_cross.hpp>
#include <spirv_glsl.hpp>

#include <rapidhash/rapidhash.h>
#include <json.hpp>


using json = nlohmann::json;

namespace Eis
{
	static bool SPIRVAvailable()
	{
		// TODO: platform capabilities
		int numFormats{};
		glGetIntegerv(GL_NUM_SHADER_BINARY_FORMATS, &numFormats);
		std::vector<GLint> formats(numFormats, 0);
		glGetIntegerv(GL_SHADER_BINARY_FORMATS, formats.data());

		// overkill, pretty sure spir-v is the only binary format in opengl

		for (GLint format : formats)
			if (format == GL_SHADER_BINARY_FORMAT_SPIR_V)
				return true;
		return false;
	}

	static GLenum ShaderTypeFromString(const std::string& type)
	{
		if (type == "vertex")
			return GL_VERTEX_SHADER;
		if (type == "fragment" || type == "pixel")
			return GL_FRAGMENT_SHADER;

		EIS_CORE_ASSERT(false, "Unknown shader type!");
		return 0;
	}
	static std::string ShaderTypeToString(GLenum stage)
	{
		switch (stage)
		{
			case GL_VERTEX_SHADER: return "vertex";
			case GL_FRAGMENT_SHADER: return "fragment";
		}
		EIS_CORE_ASSERT(false);
		return {};
	}

	static shaderc_shader_kind GLShaderStageToShaderc(GLenum stage)
	{
		switch (stage)
		{
			case GL_VERTEX_SHADER: return shaderc_shader_kind::shaderc_vertex_shader;
			case GL_FRAGMENT_SHADER: return shaderc_shader_kind::shaderc_fragment_shader;
		}
		EIS_CORE_ASSERT(false);
		return {};
	}

	static std::string GLShaderStageCacheFileExtVK(GLenum stage)
	{
		switch (stage)
		{
			case GL_VERTEX_SHADER: return ".cached_vk.vert";
			case GL_FRAGMENT_SHADER: return ".cached_vk.frag";
		}
		EIS_CORE_ASSERT(false);
		return "";
	}

	static const std::filesystem::path& GetCacheDir()
	{
		static const std::filesystem::path cacheDir{ "resources/cache/shaders/opengl" };
		return cacheDir;
	}
	static const std::filesystem::path& GetCacheRegistryPath()
	{
		static const std::filesystem::path cacheRegPath{ "resources/cache/shaders/opengl/ShaderCacheRegistry.shreg" };
		return cacheRegPath;
	}

	static void CheckCache()
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


	static bool ReadBinary(std::vector<uint32_t>& data, const std::filesystem::path& path)
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
	static void WriteBinary(const std::vector<uint32_t>& data, const std::filesystem::path& path)
	{
		std::ofstream out{ path, std::ios::binary };

		if (!out.is_open())
			return;

		out.write((const char*)data.data(), data.size() * sizeof(uint32_t));
	}


	OpenGLShader::OpenGLShader(std::string_view name, const std::string& source)
		: m_Name{ name }
	{
		EIS_PROFILE_RENDERER_FUNCTION();

		CheckCache();

		// TODO: maybe upload spir-v?
		//if (SPIRVAvailable())
		//{
		const uint64_t hash = rapidhash(source.data(), source.size());

		bool upToDate{ false };
		std::ifstream in{ GetCacheRegistryPath() };
		json j = json::parse(in);
		in.close();

		if (j.contains(m_Name))
		{
			uint64_t oldHash = j[m_Name]["Hash"].get<uint64_t>();

			if (oldHash == hash)
				upToDate = true;
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
				WriteBinary(binary, GetCacheDir() / (m_Name + GLShaderStageCacheFileExtVK(stage)));

			// Write registry
			j[m_Name]["Hash"] = hash;
			for (const auto& [stage, unused] : m_VKBinaries)
				j[m_Name]["Stages"].push_back(ShaderTypeToString(stage));

			std::ofstream out{ GetCacheRegistryPath() };
			out << j.dump(4);
		}
		else
		{
			// Load VK
			for (const auto& s : j[m_Name]["Stages"])
			{
				GLenum stage = ShaderTypeFromString(s.get<std::string>());

				auto& binary = m_VKBinaries[stage];
				ReadBinary(binary, GetCacheDir() / (m_Name + GLShaderStageCacheFileExtVK(stage)));
			}

			// cahce intermediate glsl?
			m_GLSLsources = CompileToGLSL(m_VKBinaries);
			UploadSources(m_GLSLsources);
		}
		//}
		//else
		//{
		//	ShaderSources shaderSources = PreProcess(source);
		//	EIS_CORE_WARN("SPIR-V shaders not supported! Falling back to GLSL");
		//	UploadSources(shaderSources);
		//}

		// TODO: cache reflection
		m_Reflection = Reflect(m_VKBinaries);
	}

	OpenGLShader::~OpenGLShader()
	{
		EIS_PROFILE_RENDERER_FUNCTION();

		glDeleteProgram(m_RendererId);
	}


	OpenGLShader::ShaderSources OpenGLShader::PreProcess(const std::string& source)
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
			const GLenum glType = ShaderTypeFromString(type);


			// Start of shader code after shader type declaration line
			const size_t nextLinePos = source.find_first_not_of("\r\n", eol);

			pos = source.find(typeToken, nextLinePos); // Start of next shader type declaration line

			shaderSources[glType] = (pos == std::string::npos) ? source.substr(nextLinePos) : source.substr(nextLinePos, pos - nextLinePos);
		}

		return shaderSources;
	}

	OpenGLShader::ShaderBinaries OpenGLShader::CompileToVK(const ShaderSources& glslSources, std::string_view name)
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

	OpenGLShader::ShaderSources OpenGLShader::CompileToGLSL(const ShaderBinaries& vkBinaries)
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


	static ShaderDataType ShaderDataTypeFromSPIRType(spirv_cross::SPIRType::BaseType type)
	{
		switch (type)
		{
			case spirv_cross::SPIRType::Boolean:
				return ShaderDataType::Bool;

			case spirv_cross::SPIRType::Int:
				return ShaderDataType::Int;

			case spirv_cross::SPIRType::Float:
				return ShaderDataType::Float;
		}
		
		EIS_CORE_ASSERT(false);
		return ShaderDataType::None;
	}


	static uint32_t SPIRTypeSize(const spirv_cross::SPIRType& type)
	{
		uint32_t componentSize{};
		switch (type.basetype)
		{
			case spirv_cross::SPIRType::SByte:
			case spirv_cross::SPIRType::UByte:
			case spirv_cross::SPIRType::Boolean:
				componentSize = 1; break;
			case spirv_cross::SPIRType::Short:
			case spirv_cross::SPIRType::UShort:
				componentSize = 2; break;
			case spirv_cross::SPIRType::Int:
			case spirv_cross::SPIRType::UInt:
			case spirv_cross::SPIRType::Float:
				componentSize = 4; break;
			case spirv_cross::SPIRType::Int64:
			case spirv_cross::SPIRType::UInt64:
			case spirv_cross::SPIRType::Double:
				componentSize = 8; break;
			default: EIS_CORE_ASSERT(false); break;
		}

		return componentSize * type.vecsize * type.columns;
	}

	ShaderReflection OpenGLShader::Reflect(const ShaderBinaries& binaries)
	{
		ShaderReflection reflection;


		std::vector<VertexAttribute> vertexAtribs;
		{
			const auto& vertex = binaries.at(GL_VERTEX_SHADER);
			spirv_cross::Compiler c{ vertex };
			spirv_cross::ShaderResources resources = c.get_shader_resources();

			vertexAtribs.resize(resources.stage_inputs.size());
			for (size_t i{}; i < resources.stage_inputs.size(); i++)
			{
				const auto& input = resources.stage_inputs[i];
				const auto& spvType = c.get_type(input.type_id);

				const size_t binding = c.get_decoration(input.id, spv::DecorationLocation);

				VertexAttribute& attrib = vertexAtribs[binding];
				attrib.Name = input.name;
				attrib.DataType = ShaderDataTypeFromSPIRType(spvType.basetype);
				attrib.ComponentCount = spvType.vecsize;
				attrib.Colums = spvType.columns;
				attrib.Normalized = input.name.ends_with("_n"); // maybe a better normalization method?
				attrib.Size = SPIRTypeSize(spvType);
			}
		}

		reflection.VertexAttributes = AttributeLayout{ vertexAtribs };

		return reflection;
	}



	/*
	void OpenGLShader::UploadBinaries(const ShaderBinaries& glBinaries)
	{
		EIS_PROFILE_RENDERER_FUNCTION();

		GLuint program = glCreateProgram();

		std::vector<GLuint> glShaderIDs;
		for (const auto& [stage, spirv] : glBinaries)
		{
			GLuint shader = glShaderIDs.emplace_back((GLuint)glCreateShader(stage));

			glShaderBinary(1, &shader, GL_SHADER_BINARY_FORMAT_SPIR_V, spirv.data(), (GLsizei)spirv.size() * sizeof(uint32_t));

			glSpecializeShader(shader, "main", 0, nullptr, nullptr);

			glAttachShader(program, shader);
		}

		glLinkProgram(program);

		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

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
	}*/

	void OpenGLShader::UploadSources(const ShaderSources& shaderSources)
	{
		EIS_PROFILE_RENDERER_FUNCTION();

		GLuint program = glCreateProgram();

		// TODO: consider  stack array instead of a heap vector for performance reasons
		std::vector<GLuint> glShaderIDs;
		glShaderIDs.reserve(shaderSources.size());
		for (const auto& [stage, src] : shaderSources)
		{
			GLuint shader = glCreateShader(stage);

			const GLchar* sourceCStr = src.c_str();
			glShaderSource(shader, 1, &sourceCStr, 0);

			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				glDeleteShader(shader);

				EIS_CORE_ERROR("{}", infoLog.data());
				EIS_CORE_ASSERT(false, "Shader compilation failure!");
				break;
			}

			glAttachShader(program, shader);
			glShaderIDs.push_back(shader);
		}

		glLinkProgram(program);

		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

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