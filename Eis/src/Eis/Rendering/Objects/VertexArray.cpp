#include "Eispch.h"
#include "VertexArray.h"

#include "Eis/Rendering/Renderer/RendererAPI.h"

#include "Platform/OpenGL/OpenGLVertexArray.h"
#include "Platform/WebGL/WebGLVertexArray.h"


namespace Eis
{
	Ref<VertexArray> VertexArray::Create()
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::OpenGL:
				return CreateRef<OpenGLVertexArray>();

			case RendererAPI::API::WebGL:
				return CreateRef<WebGLVertexArray>(); // TODO: check vao extension

			default:
				EIS_CORE_ASSERT(false, "Invalid graphics API: {}!", (uint8_t)RendererAPI::GetAPI());
				return nullptr;
		}
	}
}