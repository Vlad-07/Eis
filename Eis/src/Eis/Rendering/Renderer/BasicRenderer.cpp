#include "Eispch.h"
#include "BasicRenderer.h"

#include "Eis/Assets/AssetManager.h"
#include "Eis/Scene/Components.h"

#include "Eis/Rendering/Objects/Mesh.h"
#include "Eis/Rendering/Renderer/RenderCommands.h"


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

		auto meshes{ m_Scene->m_Registry.view<MeshRendererComponent>() };
		meshes.each([](const MeshRendererComponent& mrc)
			{
				Ref<StaticMesh> mesh = AssetManager::GetAsset<StaticMesh>(mrc.Mesh);

				mesh->Bind();

				for (const SubMesh& submesh : mesh->GetSubMeshes())
				{
					// TODO: render queue

					Ref<Material> material = AssetManager::GetAsset<Material>(submesh.Material);

					material->GetShader()->Bind();

					RenderCommands::DrawSubMesh(submesh);
				}
			});
	}
}