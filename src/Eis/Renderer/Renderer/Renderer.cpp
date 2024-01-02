#include "Eispch.h"

#include "Eis/Renderer/Renderer/Renderer.h"

#include "Eis/Renderer/Renderer/RenderCommands.h"
#include "Eis/Renderer/Renderer/Renderer2D.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace Eis
{
	Scope<Renderer::SceneData> Renderer::s_SceneData = CreateScope<Renderer::SceneData>();

	void Renderer::Init()
	{
		EIS_PROFILE_FUNCTION();

		RenderCommands::Init();
		Renderer2D::Init();
	}

	void Renderer::Shutdown()
	{
		EIS_PROFILE_FUNCTION();

		Renderer2D::Shutdown();
	}

	void Renderer::OnWindowResized(uint32_t width, uint32_t height)
	{
		RenderCommands::SetViewport(0, 0, width, height);
	}

	void Renderer::BeginScene(OrthographicCamera& camera)
	{
		s_SceneData->ViewProjMat = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& va, const glm::mat4& transform)
	{
		shader->Bind();
		shader->SetMat4("u_VP", s_SceneData->ViewProjMat);
		shader->SetMat4("u_Transform", transform);

		va->Bind();
		RenderCommands::DrawIndexed(va);
	}
}