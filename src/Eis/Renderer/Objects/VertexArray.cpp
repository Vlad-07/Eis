#include "Eispch.h"

#include "Eis/Renderer/Objects/VertexArray.h"

#include "Eis/Renderer/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Eis
{
	Ref<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			EIS_CORE_ASSERT(false, "Invalid graphics API: None");
			return nullptr;

		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLVertexArray>();
		}

		EIS_CORE_ASSERT(false, "Unknown graphics API");
		return nullptr;
	}
}