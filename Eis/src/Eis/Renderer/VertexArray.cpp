#include "Eispch.h"
#include "VertexArray.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Eis
{
	VertexArray* VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::None:
			EIS_CORE_ASSERT(false, "RendererAPI::None not currently suported");
			return nullptr;

		case RendererAPI::OpenGL:
			return new OpenGLVertexArray();
		}

		EIS_CORE_ASSERT(false, "Unknown API");
		return nullptr;
	}
}