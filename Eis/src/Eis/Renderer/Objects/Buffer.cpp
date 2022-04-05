#include "Eispch.h"
#include "Buffer.h"

#include "Eis/Renderer/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Eis
{
	Ref<VertexBuffer> VertexBuffer::Create(float* verticies, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			EIS_CORE_ASSERT(false, "RendererAPI::None not currently suported");
			return nullptr;

		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLVertexBuffer>(verticies, size);
		}

		EIS_CORE_ASSERT(false, "Unknown API");
		return nullptr;
	}

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indicies, uint32_t count)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			EIS_CORE_ASSERT(false, "'None' API not currently suported");
			return nullptr;

		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLIndexBuffer>(indicies, count);
		}

		EIS_CORE_ASSERT(false, "Unknown API");
		return nullptr;
	}
}