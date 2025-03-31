#include "Eispch.h"
#include "Buffers.h"

#include "Eis/Rendering/Renderer/Renderer2D.h"

#include "Platform/OpenGL/OpenGLBuffers.h"


namespace Eis
{
	Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)
	{
		switch (Renderer2D::GetAPI())
		{
		case RendererAPI::API::OpenGL:
		case RendererAPI::API::OpenGLES2:
			return CreateRef<OpenGLVertexBuffer>(size);

		default:
			EIS_CORE_ASSERT(false, "Invalid graphics API! ({0})", Renderer2D::GetAPI());
			return nullptr;
		}
	}

	Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer2D::GetAPI())
		{
		case RendererAPI::API::OpenGL:
		case RendererAPI::API::OpenGLES2:
			return CreateRef<OpenGLVertexBuffer>(vertices, size);

		default:
			EIS_CORE_ASSERT(false, "Invalid graphics API! ({0})", Renderer2D::GetAPI());
			return nullptr;
		}
	}



	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{
		switch (Renderer2D::GetAPI())
		{
		case RendererAPI::API::OpenGL:
		case RendererAPI::API::OpenGLES2:
			return CreateRef<OpenGLIndexBuffer>(indices, count);

		default:
			EIS_CORE_ASSERT(false, "Invalid graphics API! ({0})", Renderer2D::GetAPI());
			return nullptr;
		}
	}
}