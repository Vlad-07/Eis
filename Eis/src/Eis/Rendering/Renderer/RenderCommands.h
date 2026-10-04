#pragma once

#include "Eis/Rendering/Renderer/RendererAPI.h"


namespace Eis
{
	class RenderCommands
	{
	public:
		static void Init()
		{ s_RenderAPI->Init(); }

		static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
		{ s_RenderAPI->SetViewport(x, y, width, height); }

		static void SetClearColor(const glm::vec3& color)
		{ SetClearColor(glm::vec4{ color, 1.0f }); }

		static void SetClearColor(const glm::vec4& color)
		{ s_RenderAPI->SetClearColor(color); }

		static void Clear()
		{ s_RenderAPI->Clear(); }

		static void DrawSubMesh(const SubMesh& submesh)
		{ s_RenderAPI->DrawSubMesh(submesh); }

	private:
		static Scope<RendererAPI> s_RenderAPI;
	};
}