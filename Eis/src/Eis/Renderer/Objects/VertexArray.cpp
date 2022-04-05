#include "Eispch.h"
#include "VertexArray.h"

#include <Eis/Renderer/Renderer/Renderer.h>

#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Eis
{
	Ref<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			EIS_CORE_ASSERT(false, "RendererAPI::None not currently suported");
			return nullptr;

		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLVertexArray>();
		}

		EIS_CORE_ASSERT(false, "Unknown API");
		return nullptr;
	}
}