#pragma once

#include "RenderCommands.h"

namespace Eis
{
	class Renderer
	{
	public:

		static void BeginScene();
		static void EndScene();

		static void Submit(const std::shared_ptr<VertexArray>& va);

		static inline RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	};
}