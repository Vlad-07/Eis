#include "Eispch.h"
#include "RenderCommands.h"


namespace Eis
{
	Scope<RendererAPI> RenderCommands::s_RenderAPI = RendererAPI::Create();
}