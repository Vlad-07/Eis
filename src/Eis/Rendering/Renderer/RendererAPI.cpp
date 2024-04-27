#include "Eispch.h"
#include "RendererAPI.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"


namespace Eis
{
	RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;

	Scope<RendererAPI> RendererAPI::Create()
	{
		switch (s_API)
		{
		case Eis::RendererAPI::API::None:
			EIS_CORE_ASSERT(false, "Invalid graphics API: None");
			return nullptr;
		case Eis::RendererAPI::API::OpenGL:
			return CreateScope<OpenGLRendererAPI>();
		}

		EIS_CORE_ASSERT(false, "Unknown graphics API!");
		return nullptr;
	}
}