#pragma once

#include "Eis/Rendering/Objects/Shader.h"


typedef unsigned int GLuint;
typedef unsigned int GLenum;

namespace Eis
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(std::string_view name, const std::string& source, const std::vector<std::string>& defines);
		virtual ~OpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual const ShaderReflection& GetReflection() const override { return m_Reflection; }
		virtual const AttributeLayout& GetAttributeLayout() const override { return m_Reflection.VertexAttributes; }

		virtual void SetInt(const std::string& name, int value) override;
		virtual void SetIntArray(const std::string& name, const int* values, uint32_t count) override;
		virtual void SetFloat(const std::string& name, float value) override;
		virtual void SetFloat2(const std::string& name, glm::vec2 value) override;
		virtual void SetFloat3(const std::string& name, glm::vec3 value) override;
		virtual void SetFloat4(const std::string& name, glm::vec4 value) override;
		virtual void SetMat4(const std::string& name, const glm::mat4& value) override;

		//virtual const std::string& GetName() const override { return m_Name; }

	private:
		static ShaderSources CompileToGLSL(const ShaderBinaries& vkBinaries);


	//	void UploadBinaries(const ShaderBinaries& binaries);
		void UploadSources(const ShaderSources& shaderSources);

	private:
		GLuint m_RendererId{};
		std::string m_Name;

		bool m_ShouldRecompile{};

		ShaderBinaries m_VKBinaries;
		ShaderSources m_GLSLsources;

		ShaderReflection m_Reflection;
	};
}