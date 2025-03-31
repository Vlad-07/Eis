#include "Eispch.h"
#include "VertexArray.h"

#include "Eis/Rendering/Renderer/Renderer2D.h"

#include "Platform/OpenGL/OpenGLVertexArray.h"


namespace Eis
{
	Ref<VertexArray> VertexArray::Create()
	{
		switch (Renderer2D::GetAPI())
		{
		case RendererAPI::API::OpenGL:
		case RendererAPI::API::OpenGLES2:
			return CreateRef<OpenGLVertexArray>();

		default:
			EIS_CORE_ASSERT(false, "Invalid graphics API! ({0})", Renderer2D::GetAPI());
			return nullptr;
		}
	}
}