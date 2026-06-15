#include "Eispch.h"
#include "Texture.h"

#include "Eis/Rendering/Renderer/RendererAPI.h"

#include "Platform/OpenGL/OpenGLTexture.h"
#include "Platform/WebGL/WebGLTexture.h"


namespace Eis
{
	Ref<Texture2D> Texture2D::Create(const TextureSpec& spec, Buffer data)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::OpenGL:
				return CreateRef<OpenGLTexture2D>(spec, data);

			case RendererAPI::API::WebGL:
				return CreateRef<WebGLTexture2D>(spec, data);

			default:
				EIS_CORE_ASSERT(false, "Invalid graphics API: {}!", (uint8_t)RendererAPI::GetAPI());
				return nullptr;
		}
	}
}