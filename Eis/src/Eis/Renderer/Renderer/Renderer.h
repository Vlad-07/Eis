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

		static Scope<SceneData> s_SceneData;

	public:

		static void Init();
		static void OnWindowResized(uint32_t width, uint32_t height);

		static void BeginScene(OrthographicCamera& camera);
		static void EndScene();

		static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& va, const glm::mat4& transform = glm::mat4(1.0f));

		static inline RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	};
}