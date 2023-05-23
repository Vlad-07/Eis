#include "Eispch.h"

#include "Eis/Renderer/Renderer/Renderer.h"

#include "Eis/Renderer/GraphicsContext.h"
#include "Platform/OpenGL/OpenGLContext.h"

namespace Eis
{
    Scope<GraphicsContext> GraphicsContext::Create(void* window)
    {
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			EIS_CORE_ASSERT(false, "Invalid graphics API: None");
			return nullptr;

		case RendererAPI::API::OpenGL:
			return CreateScope<OpenGLContext>(static_cast<GLFWwindow*>(window));
		}

		EIS_CORE_ASSERT(false, "Unknown graphics API!");
		return nullptr;
    }
}