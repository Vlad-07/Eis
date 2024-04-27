#pragma once

#include "Eis/Renderer/Renderer/RendererAPI.h"


namespace Eis
{
	class RenderCommands
	{
	public:
		static void Init()
		{ s_RenderAPI->Init(); }

		static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
		{ s_RenderAPI->SetViewport(x, y, width, height); }

		static void SetClearColor(const glm::vec4& color)
		{ s_RenderAPI->SetClearColor(color); }

		static void Clear()
		{ s_RenderAPI->Clear(); }

		static void DrawIndexed(const Ref<VertexArray>& va, uint32_t indexCount = 0)
		{ s_RenderAPI->DrawIndex(va, indexCount); }

		static void DrawLines(const Ref<VertexArray>& va, uint32_t vertexCount = 0)
		{ s_RenderAPI->DrawLines(va, vertexCount); }

		static void SetLineWidth(float width)
		{ s_RenderAPI->SetLineWidth(width); }

		static void Enable(uint32_t code)
		{ s_RenderAPI->Enable(code); }

		static void Disable(uint32_t code)
		{ s_RenderAPI->Disable(code); }

	private:
		static Scope<RendererAPI> s_RenderAPI;
	};
}