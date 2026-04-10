#include "Eispch.h"
#include "Buffers.h"

#include "Eis/Rendering/Renderer/RendererAPI.h"

#include "Platform/OpenGL/OpenGLBuffers.h"


namespace Eis
{
	Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::OpenGL:
		case RendererAPI::API::WebGL:
			return CreateRef<OpenGLVertexBuffer>(size);

		default:
			EIS_CORE_ASSERT(false, "Invalid graphics API: {}!", (uint8_t)RendererAPI::GetAPI());
			return nullptr;
		}
	}

	Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::OpenGL:
		case RendererAPI::API::WebGL:
			return CreateRef<OpenGLVertexBuffer>(vertices, size);

		default:
			EIS_CORE_ASSERT(false, "Invalid graphics API: {}!", (uint8_t)RendererAPI::GetAPI());
			return nullptr;
		}
	}



	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::OpenGL:
		case RendererAPI::API::WebGL:
			return CreateRef<OpenGLIndexBuffer>(indices, count);

		default:
			EIS_CORE_ASSERT(false, "Invalid graphics API: {}!", (uint8_t)RendererAPI::GetAPI());
			return nullptr;
		}
	}
}