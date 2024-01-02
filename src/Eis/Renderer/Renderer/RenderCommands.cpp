#include "Eispch.h"

#include "Eis/Renderer/Renderer/RenderCommands.h"

namespace Eis
{
	Scope<RendererAPI> RenderCommands::s_RenderAPI = RendererAPI::Create();
}