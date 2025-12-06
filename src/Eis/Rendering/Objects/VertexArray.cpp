#include "Eispch.h"
#include "VertexArray.h"

#include "Eis/Rendering/Renderer/Renderer2D.h"

#include "Platform/OpenGL/OpenGLVertexArray.h"
#include "Platform/OpenGLES2/OpenGLES2VertexArray.h"


namespace Eis
{
	Ref<VertexArray> VertexArray::Create()
	{
		switch (Renderer2D::GetAPI())
		{
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLVertexArray>();

		case RendererAPI::API::OpenGLES2:
			return CreateRef<OpenGLES2VertexArray>(); // TODO: check vao extension

		default:
			EIS_CORE_ASSERT(false, "Invalid graphics API! ({0})", (uint8_t)Renderer2D::GetAPI());
			return nullptr;
		}
	}
}