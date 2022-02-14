#pragma once

#include "Eis/Renderer/Renderer/RendererAPI.h"

namespace Eis
{
	class OpenGLRendererAPI : public RendererAPI
	{
		virtual void Init() override;

		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;

		virtual void DrawIndex(const Ref<VertexArray>& va) override;

	};
}