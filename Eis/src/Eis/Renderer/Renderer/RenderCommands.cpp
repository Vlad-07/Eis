#include "Eispch.h"
#include "RenderCommands.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Eis
{
	Scope<RendererAPI> RenderCommands::s_RenderAPI = CreateScope<OpenGLRendererAPI>();
}