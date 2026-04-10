#include "Eispch.h"
#include "Framebuffer.h"

#include "Eis/Rendering/Renderer/RendererAPI.h"

#include "Platform/OpenGL/OpenGLFramebuffer.h"


namespace Eis
{
	Ref<Framebuffer> Framebuffer::Create(const FramebufferSpec& fbSpec)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::OpenGL:
		case RendererAPI::API::WebGL:
			return CreateRef<OpenGLFramebuffer>(fbSpec);

		default:
			EIS_CORE_ASSERT(false, "Invalid graphics API: {}!", (uint8_t)RendererAPI::GetAPI());
			return nullptr;
		}
	}
}