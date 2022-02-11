#include "Eispch.h"
#include "Renderer.h"

#include "RenderCommands.h"

namespace Eis
{
	void Renderer::BeginScene()
	{
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const std::shared_ptr<VertexArray>& va)
	{
		va->Bind();
		RenderCommands::DrawIndexed(va);
	}
}