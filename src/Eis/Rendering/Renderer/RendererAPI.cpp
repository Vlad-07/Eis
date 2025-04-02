#include "Eispch.h"
#include "RendererAPI.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"


namespace Eis
{
	#ifdef EIS_PLATFORM_WINDOWS
		RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;
	#elif defined(EIS_PLATFORM_WEB)
		RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGLES2;
	#endif

	Scope<RendererAPI> RendererAPI::Create()
	{
		switch (s_API)
		{
		case RendererAPI::API::OpenGL:
		case RendererAPI::API::OpenGLES2:
			return CreateScope<OpenGLRendererAPI>();

		default:
			EIS_CORE_ASSERT(false, "Invalid graphics API! ({0})", s_API);
			return nullptr;
		}
	}
}