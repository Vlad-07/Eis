#include "Eispch.h"
#include "Texture.h"

#include "Eis/Rendering/Renderer/Renderer2D.h"

#include "Platform/OpenGL/OpenGLTexture.h"
#include "Platform/WebGL/WebGLTexture.h"


namespace Eis
{
	Ref<Texture2D> Texture2D::Create(const std::string& path)
	{
		switch (Renderer2D::GetAPI())
		{
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLTexture2D>(path);

		case RendererAPI::API::WebGL:
			return CreateRef<WebGLTexture2D>(path);

		default:
			EIS_CORE_ASSERT(false, "Invalid graphics API: {}!", (uint8_t)Renderer2D::GetAPI());
			return nullptr;
		}
	}

	Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
	{
		switch (Renderer2D::GetAPI())
		{
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLTexture2D>(width, height);

		case RendererAPI::API::WebGL:
			return CreateRef<WebGLTexture2D>(width, height);

		default:
			EIS_CORE_ASSERT(false, "Invalid graphics API: {}!", (uint8_t)Renderer2D::GetAPI());
			return nullptr;
		}
	}
}