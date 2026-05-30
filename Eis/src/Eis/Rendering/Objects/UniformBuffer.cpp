#include "Eispch.h"
#include "UniformBuffer.h"

#include "Eis/Rendering/Renderer/RendererAPI.h"

#include "Platform/OpenGL/OpenGLUniformBuffer.h"


namespace Eis
{
	Ref<UniformBuffer> UniformBuffer::Create(uint64_t size, uint32_t binding)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::OpenGL:
			case RendererAPI::API::WebGL:
				return CreateRef<OpenGLUniformBuffer>(size, binding);

			default:
				EIS_CORE_ASSERT(false, "Invalid graphics API: {}!", (uint8_t)RendererAPI::GetAPI());
				return nullptr;
		}
	}
}