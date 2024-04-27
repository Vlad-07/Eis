#pragma once

#include "Eis/Rendering/Renderer/RendererAPI.h"


namespace Eis
{
	class OpenGLRendererAPI : public RendererAPI
	{
	private:
		virtual void Init() override;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);

		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;

		virtual void DrawIndex(const Ref<VertexArray>& va, uint32_t indexCount = 0) override;
		virtual void DrawLines(const Ref<VertexArray>& va, uint32_t vertexCount = 0) override;

		virtual void SetLineWidth(float width) override;

		virtual void Enable(uint32_t code) override;
		virtual void Disable(uint32_t code) override;
	};
}