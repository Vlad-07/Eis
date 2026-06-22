#include "Eispch.h"
#include "SceneRenderer.h"

#include "RenderCommands.h"

#include "Eis/Rendering/Objects/VertexArray.h"
#include "Eis/Rendering/Objects/Mesh.h"
#include "Eis/Rendering/Objects/VertexBufferData.h"
#include "Eis/Rendering/Objects/Shader.h"
#include "Eis/Rendering/Objects/Framebuffer.h"

#include "Eis/Assets/Importers/ShaderImporter.h"

#include <glad/glad.h>


namespace Eis
{
	struct RendererData
	{
//		Ref<Framebuffer> ScreenBuffer;

		struct CameraData
		{
			glm::mat4 ViewProjection{};
		} CameraBuf{};
		Ref<UniformBuffer> CameraUniformBuf;

		Ref<Shader> MeshShader;
	};
	static RendererData s_Data;

	void SceneRenderer::Init()
	{
		// Init pipeline

		FramebufferSpec screenBufferSpec;
		screenBufferSpec.SwapChainTarget = true;
//		s_Data.ScreenBuffer = Framebuffer::Create(screenBufferSpec);

		// Init shaders

		s_Data.MeshShader = ShaderImporter::LoadShader("resources/shaders/Mesh.glsl");

		s_Data.CameraUniformBuf = UniformBuffer::Create(sizeof(RendererData::CameraData), 0);
	}



	void SceneRenderer::BeginScene(const Camera& cam, const glm::mat4& transform)
	{
		s_Data.CameraBuf.ViewProjection = cam.GetProjection() * glm::inverse(transform);
		s_Data.CameraUniformBuf->SetData(&s_Data.CameraBuf, sizeof(RendererData::CameraData));

		Pass();
	}

	void SceneRenderer::BeginScene(const EditorCamera2& cam)
	{
		s_Data.CameraBuf.ViewProjection = cam.GetViewProjection();
		s_Data.CameraUniformBuf->SetData(&s_Data.CameraBuf, sizeof(RendererData::CameraData));

		Pass();
	}

	void SceneRenderer::EndScene()
	{

	}


	void SceneRenderer::Pass()
	{
//		glEnable(GL_DEPTH_TEST);

//		s_Data.ScreenBuffer->Bind();
	}



	void SceneRenderer::DrawMesh(const glm::mat4& transform, const Ref<Mesh>& mesh)
	{
		s_Data.MeshShader->Bind();

		for (const SubMesh& submesh : mesh->GetSubMeshes())
			RenderCommands::DrawIndexed(mesh->GetVA(), submesh.IndexCount, submesh.FirstIndex);
	}
}