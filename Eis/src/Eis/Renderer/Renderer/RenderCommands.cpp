#include "Eispch.h"
#include "RenderCommands.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h";

namespace Eis
{
	RendererAPI* RenderCommands::s_RenderAPI = new OpenGLRendererAPI;
}