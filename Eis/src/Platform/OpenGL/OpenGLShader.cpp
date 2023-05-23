#include "Eispch.h"
#include "OpenGLShader.h"

#include <ios>
#include <fstream>

#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>

namespace Eis
{
	static GLenum ShaderTypeFromString(const std::string& type)
	{
		if (type == "vertex")	return GL_VERTEX_SHADER;
		if (type == "fragment") return GL_FRAGMENT_SHADER;

		EIS_CORE_ASSERT(false, "Unknown shader type!");
		return 0;
	}


	OpenGLShader::OpenGLShader(const std::string& filePath)
	{
		EIS_PROFILE_RENDERER_FUNCTION();

		std::string source = ReadFile(filePath);
		auto shaderSources = PreProcess(source);
		Compile(shaderSources);

		// Extract name
		// From "assets/textures/CoolShader.glsl"
		// we get "CoolShader"
		auto lastSlash = filePath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = filePath.rfind('.');
		auto count = lastDot == std::string::npos ? filePath.size() - lastSlash : lastDot - lastSlash;

		m_Name = filePath.substr(lastSlash, count);
	}

	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vsSrc, const std::string& fsSrc)
		: m_Name(name)
	{
		EIS_PROFILE_RENDERER_FUNCTION();

		std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = vsSrc;
		sources[GL_FRAGMENT_SHADER] = fsSrc;
		Compile(sources);
	}

	OpenGLShader::~OpenGLShader()
	{
		EIS_PROFILE_RENDERER_FUNCTION();

		glDeleteProgram(m_RendererId);
	}


	std::string OpenGLShader::ReadFile(const std::string& filePath)
	{
		EIS_PROFILE_RENDERER_FUNCTION();

		std::string result;
		std::ifstream in(filePath, std::ios::in | std::ios::binary);

		if (in)
		{
			in.seekg(0, std::ios::end);
			size_t size = in.tellg();
			if (size != -1)
			{
				result.resize(size);
				in.seekg(0, std::ios::beg);
				in.read(&result[0], size);
				in.close();
			}
			else
				EIS_CORE_ERROR("Could not read from file '{0}'", filePath);
		}
		else
			EIS_CORE_WARN("Could not open file '{0}'", filePath);

		return result;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source)
	{
		EIS_PROFILE_RENDERER_FUNCTION();

		std::unordered_map<GLenum, std::string> shaderSources;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0); //Start of shader type declaration line
		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos); //End of shader type declaration line
			EIS_CORE_ASSERT(eol != std::string::npos, "Syntax error");

			size_t begin = pos + typeTokenLength + 1; //Start of shader type name (after "#type" keyword)
			std::string type = source.substr(begin, eol - begin);
			EIS_CORE_ASSERT(ShaderTypeFromString(type), "Invalid shader type specified");

			size_t nextLinePos = source.find_first_not_of("\r\n", eol); //Start of shader code after shader type declaration line
			EIS_CORE_ASSERT(nextLinePos != std::string::npos, "Syntax error");
			pos = source.find(typeToken, nextLinePos); //Start of next shader type declaration line

			shaderSources[ShaderTypeFromString(type)] = (pos == std::string::npos) ? source.substr(nextLinePos) : source.substr(nextLinePos, pos - nextLinePos);
		}

		return shaderSources;
	}

	void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string>& shaderSources)
	{
		EIS_PROFILE_RENDERER_FUNCTION();

		GLuint program = glCreateProgram();

		// TODO: consider  stack array instead of a heap vector for performance reasons
		std::vector<GLenum> glShaderIDs;
		glShaderIDs.reserve(shaderSources.size());
		for (auto& keyVal : shaderSources)
		{
			GLenum type = keyVal.first;
			const std::string& source = keyVal.second;

			GLuint shader = glCreateShader(type);

			const GLchar* sourceCStr = source.c_str();
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

				EIS_CORE_ERROR("{0}", infoLog.data());
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

			EIS_CORE_ERROR("{0}", infoLog.data());
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



	void OpenGLShader::SetInt(const std::string& name, const int& value)
	{
		EIS_PROFILE_RENDERER_FUNCTION();

		UploadUniformInt(name, value);
	}

	void OpenGLShader::SetFloat(const std::string& name, const float& value)
	{
		EIS_PROFILE_RENDERER_FUNCTION();

		UploadUniformFloat(name, value);
	}

	void OpenGLShader::SetFloat2(const std::string& name, const glm::vec2& value)
	{
		EIS_PROFILE_RENDERER_FUNCTION();

		UploadUniformFloat2(name, value);
	}

	void OpenGLShader::SetFloat3(const std::string& name, const glm::vec3& value)
	{
		EIS_PROFILE_RENDERER_FUNCTION();

		UploadUniformFloat3(name, value);
	}

	void OpenGLShader::SetFloat4(const std::string& name, const glm::vec4& value)
	{
		EIS_PROFILE_RENDERER_FUNCTION();

		UploadUniformFloat4(name, value);
	}

	void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& value)
	{
		EIS_PROFILE_RENDERER_FUNCTION();

		UploadUniformMat4(name, value);
	}


	void OpenGLShader::UploadUniformInt(const std::string& name, const int& value)
	{
		int32_t location = glGetUniformLocation(m_RendererId, name.c_str());
		glUniform1i(location, value);
	}

	void OpenGLShader::UploadUniformFloat(const std::string& name, const float& value)
	{
		int32_t location = glGetUniformLocation(m_RendererId, name.c_str());
		glUniform1f(location, value);
	}

	void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& value)
	{
		int32_t location = glGetUniformLocation(m_RendererId, name.c_str());
		glUniform2f(location, value.x, value.y);
	}

	void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& value)
	{
		int32_t location = glGetUniformLocation(m_RendererId, name.c_str());
		glUniform3f(location, value.x, value.y, value.z);
	}

	void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& value)
	{
		int32_t location = glGetUniformLocation(m_RendererId, name.c_str());
		glUniform4f(location, value.x, value.y, value.z, value.w);
	}

	void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix)
	{
		int32_t location = glGetUniformLocation(m_RendererId, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix)
	{
		int32_t location = glGetUniformLocation(m_RendererId, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}	
}