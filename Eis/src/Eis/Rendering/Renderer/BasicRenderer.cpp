#include "Eispch.h"
#include "BasicRenderer.h"

#include "Eis/Assets/AssetManager.h"
#include "Eis/Scene/Components.h"

#include "Eis/Rendering/Objects/Mesh.h"
#include "Eis/Rendering/Renderer/RenderCommands.h"

// TODO: proper api
#include <glad/glad.h>


namespace Eis
{
	BasicRenderer::BasicRenderer()
	{
		// Init pipeline

		FramebufferSpec screenBufferSpec;
		screenBufferSpec.SwapChainTarget = true;


		m_Data.SceneDataUniformBuf = UniformBuffer::Create(sizeof(BasicRendererData::SceneData), 0);

		m_Data.ObjectDataBuf = UniformBuffer::Create(sizeof(BasicRendererData::ObjectData), 2);
	}


	void BasicRenderer::Render()
	{
		if (!m_Scene) return;

		// gather cameras...

		//m_Data.SceneData.ViewProjection = cam.GetProjection() * glm::inverse(transform);
		//m_Data.SceneDataUniformBuf->SetData(&s_Data.CameraBuf, sizeof(RendererData::CameraData));
	}

	void BasicRenderer::RenderEditor(const EditorCamera& cam)
	{
		if (!m_Scene) return;

		m_Data.SceneData.ViewProjection = cam.GetViewProjection();
		m_Data.SceneDataUniformBuf->SetData(&m_Data.SceneData, sizeof(BasicRendererData::SceneData));

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		auto meshes{ m_Scene->m_Registry.view<MeshRendererComponent, TransformComponent>() };
		meshes.each([&](const MeshRendererComponent& mrc, const TransformComponent& transform)
			{
				Ref<StaticMesh> mesh = AssetManager::GetAsset<StaticMesh>(mrc.Mesh);

				mesh->Bind();

				m_Data.ObjectData.Model = transform.GetTransform();
				m_Data.ObjectDataBuf->SetData(&m_Data.ObjectData, sizeof(BasicRendererData::ObjectData));

				for (const SubMesh& submesh : mesh->GetSubMeshes())
				{
					// TODO: render queue

					Ref<MaterialBase> material = AssetManager::GetAsset<MaterialBase>(submesh.Material);

					material->Bind();

					RenderCommands::DrawSubMesh(submesh);
				}
			});
	}
}