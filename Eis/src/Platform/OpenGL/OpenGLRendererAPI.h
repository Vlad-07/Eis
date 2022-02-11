#pragma once

#include "Eis/Renderer/RendererAPI.h"

namespace Eis
{
	class OpenGLRendererAPI : public RendererAPI
	{
		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;

		virtual void DrawIndex(const std::shared_ptr<VertexArray>& va) override;

	};
}