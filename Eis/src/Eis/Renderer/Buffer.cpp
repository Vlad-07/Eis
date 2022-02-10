#include "Eispch.h"
#include "Buffer.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Eis
{
	VertexBuffer* VertexBuffer::Create(float* verticies, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::None:
			EIS_CORE_ASSERT(false, "RendererAPI::None not currently suported");
			return nullptr;

		case RendererAPI::OpenGL:
			return new OpenGLVertexBuffer(verticies, size);
		}

		EIS_CORE_ASSERT(false, "Unknown API");
		return nullptr;
	}

	IndexBuffer* IndexBuffer::Create(uint32_t* indicies, uint32_t count)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::None:
			EIS_CORE_ASSERT(false, "'None' API not currently suported");
			return nullptr;

		case RendererAPI::OpenGL:
			return new OpenGLIndexBuffer(indicies, count);
		}

		EIS_CORE_ASSERT(false, "Unknown API");
		return nullptr;
	}
}