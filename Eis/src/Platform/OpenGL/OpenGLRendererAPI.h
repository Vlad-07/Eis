#pragma once


#include "Eis/Core/Core.h"
#include "Eis/Rendering/Renderer/RendererAPI.h"


namespace Eis
{
	class OpenGLRendererAPI : public RendererAPI
	{
	private:
		virtual void Init() override;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;

		virtual void DrawSubMesh(const SubMesh& submesh) override;
	};
}