#pragma once


namespace Eis
{
	enum class RendererAPI
	{
		None = 0,
		OpenGL
	};

	class Renderer
	{
	private:
		static RendererAPI s_RendererAPI;

	public:
		static inline RendererAPI GetAPI() { return s_RendererAPI; }

	};
}