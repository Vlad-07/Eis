#pragma once

#include <glm/glm.hpp>


namespace Eis
{
	class VertexArray;

	class RendererAPI
	{
	public:
		// TODO: maybe a fmt formater
		enum class API
		{
			None = 0, OpenGL, WebGL
		};

	public:
		virtual void Init() = 0;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;

		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;

		virtual void DrawIndexed(const Ref<VertexArray>& va, uint32_t indexCount = 0, uint32_t firstIndex = 0) = 0;
		virtual void DrawArrays(const Ref<VertexArray>& va, uint32_t vertexCount) = 0;
		virtual void DrawLines(const Ref<VertexArray>& va, uint32_t vertexCount = 0) = 0;

		virtual void SetLineWidth(float width) = 0;

		virtual void Enable(uint32_t code) = 0;
		virtual void Disable(uint32_t code) = 0;

		static API GetAPI() { return s_API; }
		static Scope<RendererAPI> Create();

	private:
		static API s_API;
	};
}