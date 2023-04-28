#pragma once

#include "RendererAPI.h"

namespace Eis
{
	class RenderCommands
	{
	public:
		inline static void Init()
		{ s_RenderAPI->Init(); }

		inline static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
		{ s_RenderAPI->SetViewport(x, y, width, height); }

		inline static void SetClearColor(const glm::vec4& color)
		{ s_RenderAPI->SetClearColor(color); }

		inline static void Clear()
		{ s_RenderAPI->Clear(); }

		inline static void DrawIndexed(const Ref<VertexArray>& va)
		{ s_RenderAPI->DrawIndex(va); }

		inline static void Enable(uint32_t code)
		{ s_RenderAPI->Enable(code); }

		inline static void Disable(uint32_t code)
		{ s_RenderAPI->Disable(code); }

	private:
		static Scope<RendererAPI> s_RenderAPI;
	};
}