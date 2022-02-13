#pragma once

#include <glm/glm.hpp>
#include <Eis/Renderer/Objects/VertexArray.h>

namespace Eis
{
	class RendererAPI
	{
	public:
		enum class API
		{
			None = 0,
			OpenGL
		};

	public:
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;

		virtual void DrawIndex(const Ref<VertexArray>& va) = 0;

		static inline API GetAPI() { return s_API; }

	private:
		static API s_API;
	};
}