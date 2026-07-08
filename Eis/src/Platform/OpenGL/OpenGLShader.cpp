#include "Eispch.h"
#include "OpenGLShader.h"

#include "Eis/Rendering/Objects/ShaderCompiler.h"
#include "Eis/Rendering/Objects/ShaderReflector.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include <spirv_cross.hpp>
#include <spirv_glsl.hpp>


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
	}


	OpenGLShader::OpenGLShader(std::string_view name, const std::string& source, const std::vector<std::string>& defines)
		: m_Name{ name }
	{
		EIS_PROFILE_RENDERER_FUNCTION();

		m_VKBinaries = ShaderCompiler::Compile(source, m_Name, defines);
		m_GLSLsources = CompileToGLSL(m_VKBinaries);
		UploadSources(m_GLSLsources);

		// cache glsl and reflection too?

		m_Reflection = ShaderReflector::Reflect(m_VKBinaries);
	}

	OpenGLShader::~OpenGLShader()
	{
		EIS_PROFILE_RENDERER_FUNCTION();

		glDeleteProgram(m_RendererId);
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