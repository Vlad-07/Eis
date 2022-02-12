#pragma once

#include <memory>
#include "RenderCommands.h"
#include "Eis/Renderer/Objects/Shader.h"
#include "Eis/Renderer/Objects/OrthographicCamera.h"

namespace Eis
{
	class Renderer
	{
	private:
		struct SceneData
		{
			glm::mat4 ViewProjMat;
		};

		static SceneData* s_SceneData;

	public:

		static void BeginScene(OrthographicCamera& camera);
		static void EndScene();

		static void Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& va);

		static inline RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	};
}