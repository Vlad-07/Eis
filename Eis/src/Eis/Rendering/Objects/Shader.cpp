#include "Eispch.h"
#include "Shader.h"

#include "Eis/Rendering/Renderer/RendererAPI.h"

#include "Platform/OpenGL/OpenGLShader.h"


namespace Eis
{
	Ref<Shader> Shader::Create(const std::filesystem::path& path)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::OpenGL:
			case RendererAPI::API::WebGL:
				return CreateRef<OpenGLShader>(path);

			default:
				EIS_CORE_ASSERT(false, "Invalid graphics API: {}!", (uint8_t)RendererAPI::GetAPI());
				return nullptr;
		}
	}


	/*Ref<Shader> Shader::Create(const std::string& name, const std::string& vsSrc, const std::string& fsSrc)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::OpenGL:
			case RendererAPI::API::WebGL:
				return CreateRef<OpenGLShader>(name, vsSrc, fsSrc);

			default:
				EIS_CORE_ASSERT(false, "Invalid graphics API: {}!", (uint8_t)RendererAPI::GetAPI());
				return nullptr;
		}
	}*/
}