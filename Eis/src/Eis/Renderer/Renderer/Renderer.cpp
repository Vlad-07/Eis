#include "Eispch.h"
#include "Renderer.h"

#include "RenderCommands.h"

namespace Eis
{
	Renderer::SceneData* Renderer::s_SceneData = new Renderer::SceneData;

	void Renderer::BeginScene(OrthographicCamera& camera)
	{
		s_SceneData->ViewProjMat = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& va)
	{
		shader->Bind();
		shader->UploadUniformMat4("u_VP", s_SceneData->ViewProjMat);

		va->Bind();
		RenderCommands::DrawIndexed(va);
	}
}