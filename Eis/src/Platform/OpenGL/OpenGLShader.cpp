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


namespace Eis
{
	static GLenum ShaderTypeFromString(const std::string& type)
	{
		if (type == "vertex")
			return GL_VERTEX_SHADER;
		if (type == "fragment" || type == "pixel")
			return GL_FRAGMENT_SHADER;

		EIS_CORE_ASSERT(false, "Unknown shader type!");
		return 0;
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

	static std::string GLShaderStageCacheFileExtGL(GLenum stage)
	{
		switch (stage)
		{
			case GL_VERTEX_SHADER: return ".cached_gl.vert";
			case GL_FRAGMENT_SHADER: return ".cached_gl.frag";
		}
		EIS_CORE_ASSERT(false);
		return "";
	}

	static std::filesystem::path GetCacheDir()
	{
		return "resources/cache/shaders/opengl";
	}

	static void CreateCacheDir()
	{
		const std::filesystem::path& path = GetCacheDir();
		if (!std::filesystem::exists(path))
			std::filesystem::create_directories(path);
	}


	OpenGLShader::OpenGLShader(const std::filesystem::path& path)
	{
		EIS_PROFILE_RENDERER_FUNCTION();

		m_Name = path.filename().string();

		std::string source = ReadFile(path);
		auto shaderSources = PreProcess(source);

		// TODO: platform capabilities
		int numFormats = 0;
		glGetIntegerv(GL_NUM_SHADER_BINARY_FORMATS, &numFormats);
		GLint* formats = new GLint[numFormats];
		glGetIntegerv(GL_SHADER_BINARY_FORMATS, formats);
		if (formats[0] == GL_SHADER_BINARY_FORMAT_SPIR_V)
		{
			CreateCacheDir();

			CompileToVK(shaderSources);
			TranspileToGL();
			UploadBinaries();
		}
		else
			CompileGLSL(shaderSources);
	}

	/*OpenGLShader::OpenGLShader(const std::string& name, const std::string& vsSrc, const std::string& fsSrc)
		: m_Name(name)
	{
		EIS_PROFILE_RENDERER_FUNCTION();

		EIS_CORE_INFO("Compiling shader: {}", m_Name);

		std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = vsSrc;
		sources[GL_FRAGMENT_SHADER] = fsSrc;

		CompileGLSL(sources);
	}*/

	OpenGLShader::~OpenGLShader()
	{
		EIS_PROFILE_RENDERER_FUNCTION();

		glDeleteProgram(m_RendererId);
	}


	std::string OpenGLShader::ReadFile(const std::filesystem::path& path)
	{
		EIS_PROFILE_RENDERER_FUNCTION();

		std::ifstream in(path, std::ios::binary);

		if (!in)
		{
			EIS_CORE_ERROR("Could not open file '{}'", path.string());
			return {};
		}

		in.seekg(0, std::ios::end);
		size_t size = in.tellg();
		in.seekg(0, std::ios::beg);

		if (size == -1)
		{
			EIS_CORE_ERROR("Could not read from file '{}'", path.string());
			return {};
		}

		std::string result(size, '\0');
		in.read(result.data(), size);
		return result;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source)
	{
		EIS_PROFILE_RENDERER_FUNCTION();

		std::unordered_map<GLenum, std::string> shaderSources;

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

	void OpenGLShader::CompileToVK(const std::unordered_map<GLenum, std::string>& shaderSources)
	{
		shaderc::Compiler compiler;
		shaderc::CompileOptions options;
		options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_4);
		options.SetOptimizationLevel(shaderc_optimization_level_zero);

		std::filesystem::path cacheDir = GetCacheDir();

		m_VKBinaries.clear();
		for (const auto& [stage, src] : shaderSources)
		{
			const std::filesystem::path cachePath = GetCacheDir() / (m_Name + GLShaderStageCacheFileExtVK(stage));

			std::vector<uint32_t>& stageData = m_VKBinaries[stage];

			std::ifstream in{ cachePath, std::ios::binary };
			if (in.is_open())
			{
				in.seekg(0, std::ios::end);
				size_t size = in.tellg();
				in.seekg(0, std::ios::beg);

				stageData.resize(size / sizeof(uint32_t));
				in.read((char*)stageData.data(), size);
			}
			else
			{
				shaderc::SpvCompilationResult module
					= compiler.CompileGlslToSpv(src, GLShaderStageToShaderc(stage), m_Name.c_str(), options);
				if (module.GetCompilationStatus() != shaderc_compilation_status_success)
				{
					EIS_CORE_ERROR("GLSL to SPIR-V compilation failed!");
					EIS_CORE_ERROR("{}", module.GetErrorMessage());
					EIS_CORE_ASSERT(false);
				}

				stageData.assign(module.cbegin(), module.cend());

				std::ofstream out{ cachePath, std::ios::binary };
				if (out.is_open())
				{
					out.write((char*)stageData.data(), stageData.size() * sizeof(uint32_t));
					out.flush();
					out.close();
				}
			}
		}
	}

	void OpenGLShader::TranspileToGL()
	{
		shaderc::Compiler compiler;
		shaderc::CompileOptions options;
		options.SetTargetEnvironment(shaderc_target_env_opengl, shaderc_env_version_opengl_4_5);
		options.SetOptimizationLevel(shaderc_optimization_level_zero);

		std::filesystem::path cacheDir = GetCacheDir();

		for (const auto& [stage, spirv] : m_VKBinaries)
		{
			const std::filesystem::path cachePath = GetCacheDir() / (m_Name + GLShaderStageCacheFileExtGL(stage));

			std::vector<uint32_t>& stageData = m_GLBinaries[stage];

			std::ifstream in{ cachePath, std::ios::binary };
			if (in)
			{
				in.seekg(0, std::ios::end);
				size_t size = in.tellg();
				in.seekg(0, std::ios::beg);

				stageData.resize(size / sizeof(uint32_t));
				in.read((char*)stageData.data(), size);
			}
			else
			{
				spirv_cross::CompilerGLSL::Options crossOptions;
				crossOptions.vulkan_semantics = true; // might be bad?

				spirv_cross::CompilerGLSL glslCompiler{ spirv };
				glslCompiler.set_common_options(crossOptions);

				std::string glslSrc = glslCompiler.compile();

				//EIS_CORE_TRACE("{}", glslSrc);

				shaderc::SpvCompilationResult module =
					compiler.CompileGlslToSpv(glslSrc, GLShaderStageToShaderc(stage), m_Name.c_str(), options);
				if (module.GetCompilationStatus() != shaderc_compilation_status_success)
				{
					EIS_CORE_ERROR("Transpiled GLSL to SPIR-V compilation failed!");
					EIS_CORE_ERROR("{}", module.GetErrorMessage());
					EIS_CORE_ASSERT(false);
				}

				stageData.assign(module.cbegin(), module.cend());

				std::ofstream out{ cachePath, std::ios::binary };
				if (out.is_open())
				{
					out.write((char*)stageData.data(), stageData.size() * sizeof(uint32_t));
					out.flush();
					out.close();
				}
			}
		}
	}

	void OpenGLShader::UploadBinaries()
	{
		EIS_PROFILE_RENDERER_FUNCTION();

		GLuint program = glCreateProgram();

		std::vector<GLuint> glShaderIDs;
		for (const auto& [stage, spirv] : m_GLBinaries)
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
	}

	void OpenGLShader::Reflect(GLenum stage, std::vector<uint32_t> spirv)
	{
		spirv_cross::Compiler compiler{ spirv };
		spirv_cross::ShaderResources res{ compiler.get_shader_resources() };

		// do reflection...
	}


	void OpenGLShader::CompileGLSL(const std::unordered_map<GLenum, std::string>& shaderSources)
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