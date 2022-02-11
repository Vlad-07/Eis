#pragma once

#include "RendererAPI.h"

namespace Eis
{
	class RenderCommands
	{
	private:
		static RendererAPI* s_RenderAPI;

	public:

		inline static void SetClearColor(const glm::vec4& color)
		{ s_RenderAPI->SetClearColor(color); }

		inline static void Clear()
		{ s_RenderAPI->Clear(); }

		inline static void DrawIndexed(const std::shared_ptr<VertexArray>& va)
		{ s_RenderAPI->DrawIndex(va); }

	};
}