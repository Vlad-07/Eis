#include "Eispch.h"
#include "SceneRenderer.h"

#include "RenderCommands.h"

#include "Eis/Rendering/Objects/VertexArray.h"
#include "Eis/Rendering/Objects/Mesh.h"
#include "Eis/Rendering/Objects/Texture.h"
#include "Eis/Rendering/Objects/VertexBufferData.h"
#include "Eis/Rendering/Objects/Shader.h"
#include "Eis/Rendering/Objects/Framebuffer.h"

#include "Eis/Assets/AssetManager.h"
#include "Eis/Assets/Importers/ShaderImporter.h"

#include <glad/glad.h>


namespace Eis
{
	struct RendererData
	{
		struct SceneData
		{
			glm::mat4 ViewProjection{};
		} SceneData{};
		//Ref<UniformBuffer> SceneDataUniformBuf;

		struct ObjectData
		{
			glm::mat4 Model{};
		} ObjectData;
		Ref<UniformBuffer> ObjectDataBuf;

		Ref<Shader> MeshShader;
	};
	static RendererData s_Data;

	void SceneRenderer::Init()
	{
		// Init pipeline

		FramebufferSpec screenBufferSpec;
		screenBufferSpec.SwapChainTarget = true;

		// Init shaders

		s_Data.MeshShader = ShaderImporter::LoadShader("resources/shaders/Mesh.glsl");

		// TODO: Leeching off the Renderer2D state for now...
		//s_Data.CameraUniformBuf = UniformBuffer::Create(sizeof(RendererData::CameraData), 0);

		s_Data.ObjectDataBuf = UniformBuffer::Create(sizeof(RendererData::ObjectData), 2);
	}



	void SceneRenderer::BeginScene(const Camera& cam, const glm::mat4& transform)
	{
		//s_Data.CameraBuf.ViewProjection = cam.GetProjection() * glm::inverse(transform);
		//s_Data.CameraUniformBuf->SetData(&s_Data.CameraBuf, sizeof(RendererData::CameraData));

		Pass();
	}

	void SceneRenderer::BeginScene(const EditorCamera2& cam)
	{
		//s_Data.CameraBuf.ViewProjection = cam.GetViewProjection();
		//s_Data.CameraUniformBuf->SetData(&s_Data.CameraBuf, sizeof(RendererData::CameraData));

		Pass();
	}

	void SceneRenderer::EndScene()
	{

	}


	void SceneRenderer::Pass()
	{
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}



	void SceneRenderer::DrawMesh(const glm::mat4& transform, const Ref<Mesh>& mesh)
	{
		s_Data.MeshShader->Bind();

		s_Data.ObjectData.Model = transform;
		s_Data.ObjectDataBuf->SetData(&s_Data.ObjectData, sizeof(RendererData::ObjectData));

		for (const SubMesh& submesh : mesh->GetSubMeshes())
		{
			if (submesh.Tex)
				AssetManager::GetAsset<Texture2D>(submesh.Tex)->Bind(1);
			RenderCommands::DrawIndexed(mesh->GetVA(), submesh.IndexCount, submesh.FirstIndex);
		}
	}
}