#pragma once

#include "Eis/Rendering/Objects/Mesh.h"
#include "Eis/Rendering/Objects/Material.h"

#include <glm/glm.hpp>


namespace Eis
{
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

		virtual void DrawSubMesh(const SubMesh& submesh) = 0;


		static API GetAPI() { return s_API; }
		static Scope<RendererAPI> Create();

	private:
		static API s_API;
	};
}