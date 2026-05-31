#include "Eispch.h"
#include "Shader.h"

#include "Eis/Rendering/Renderer/RendererAPI.h"

#include "Platform/OpenGL/OpenGLShader.h"


namespace Eis
{
	Ref<Shader> Shader::Create(std::string_view name, const std::string& source)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::OpenGL:
			case RendererAPI::API::WebGL:
				return CreateRef<OpenGLShader>(name, source);

			default:
				EIS_CORE_ASSERT(false, "Invalid graphics API: {}!", (uint8_t)RendererAPI::GetAPI());
				return nullptr;
		}
	}
}