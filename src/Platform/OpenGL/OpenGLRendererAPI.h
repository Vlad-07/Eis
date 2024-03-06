#pragma once

#include "Eis/Renderer/Renderer/RendererAPI.h"


namespace Eis
{
	class OpenGLRendererAPI : public RendererAPI
	{
	private:
		virtual void Init() override;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);

		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;

		virtual void DrawIndex(const Ref<VertexArray>& va) override;
		virtual void DrawLines(const Ref<VertexArray>& va, const float& width) override;

		virtual void Enable(uint32_t code) override;
		virtual void Disable(uint32_t code) override;
	};
}