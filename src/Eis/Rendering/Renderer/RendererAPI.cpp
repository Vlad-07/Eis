#include "Eispch.h"
#include "RendererAPI.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"


namespace Eis
{
	RendererAPI::API RendererAPI::s_API =
#ifdef EIS_PLATFORM_WINDOWS
		RendererAPI::API::OpenGL;
#elif defined(EIS_PLATFORM_WEB)
		RendererAPI::API::WebGL;
#endif

	Scope<RendererAPI> RendererAPI::Create()
	{
		switch (s_API)
		{
		case RendererAPI::API::OpenGL:
		case RendererAPI::API::WebGL:
			return CreateScope<OpenGLRendererAPI>();

		default:
			EIS_CORE_ASSERT(false, "Invalid graphics API: {}!", (uint8_t)s_API);
			return nullptr;
		}
	}
}