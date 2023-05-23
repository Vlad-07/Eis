#include "Eispch.h"

#include "Eis/Renderer/Objects/Shader.h"

#include "Eis/Renderer/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace Eis
{
	Ref<Shader> Shader::Create(const std::string& filepath)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			EIS_CORE_ASSERT(false, "Invalid graphics API: None");
			return nullptr;

		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLShader>(filepath);
		}

		EIS_CORE_ASSERT(false, "Unknown graphics API");
		return nullptr;
	}

	Ref<Shader> Shader::Create(const std::string& name, const std::string& vsSrc, const std::string& fsSrc)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			EIS_CORE_ASSERT(false, "Invalid graphics API: None");
			return nullptr;

		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLShader>(name, vsSrc, fsSrc);
		}

		EIS_CORE_ASSERT(false, "Unknown graphics API");
		return nullptr;
	}



	

	void ShaderLibrary::Add(const std::string& name, const Ref<Shader>& shader)
	{
		EIS_CORE_ASSERT(!Exists(name), "Shader already exists!");
		m_Shaders[name] = shader;
	}

	void ShaderLibrary::Add(const Ref<Shader>& shader)
	{
		auto& name = shader->GetName();
		EIS_CORE_ASSERT(!Exists(name), "Shader already exists!");
		m_Shaders[name] = shader;
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& filePath)
	{
		auto& shader = Shader::Create(filePath);
		Add(shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::string& filePath)
	{
		auto shader = Shader::Create(filePath);
		Add(name, shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Get(const std::string& name)
	{
		EIS_CORE_ASSERT(Exists(name), "Shader not found!");
		return m_Shaders[name];
	}

	bool ShaderLibrary::Exists(const std::string& name)
	{
		return m_Shaders.find(name) != m_Shaders.end();
	}
}