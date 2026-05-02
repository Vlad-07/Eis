#include "Eispch.h"
#include "GraphicsContext.h"

#include "Eis/Rendering/Renderer/RendererAPI.h"

#include "Platform/OpenGL/OpenGLContext.h"
#include "Platform/WebGL/WebGLContext.h"


namespace Eis
{
    Scope<GraphicsContext> GraphicsContext::Create(void* window)
    {
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::OpenGL:
			return CreateScope<OpenGLContext>(static_cast<GLFWwindow*>(window));

		case RendererAPI::API::WebGL:
			return CreateScope<WebGLContext>(static_cast<GLFWwindow*>(window));

		default:
			EIS_CORE_ASSERT(false, "Invalid graphics API: {}!", (uint8_t)RendererAPI::GetAPI());
			return nullptr;
		}
    }
}