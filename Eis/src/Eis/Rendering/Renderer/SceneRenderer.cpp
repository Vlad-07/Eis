#include "Eispch.h"
#include "SceneRenderer.h"

#include "RenderCommands.h"

#include "Eis/Rendering/Objects/Camera.h"
#include "Eis/Rendering/Objects/Framebuffer.h"
#include "Eis/Rendering/Objects/VertexArray.h"
#include "Eis/Rendering/Objects/VertexBufferData.h"
#include "Eis/Rendering/Objects/Shader.h"
#include "Eis/Rendering/Objects/Texture.h"
#include "Eis/Rendering/Objects/Mesh.h"
#include "Eis/Rendering/Objects/EditorCamera2.h"

#include "Eis/Assets/AssetManager.h"
#include "Eis/Assets/Importers/ShaderImporter.h"

#include <glad/glad.h>


namespace Eis
{
	struct RendererData
	{
		struct Plane
		{
			glm::vec3 Normal{};
			float Dist{};
		};
		std::array<Plane, 6> FrustumPlanes;

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

		CalcFrustumPlanes(cam.GetViewProjection());

		

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

		s_Data.MeshShader->Bind();
	}



	void SceneRenderer::DrawMesh(const glm::mat4& transform, const Ref<StaticMesh>& mesh)
	{
		if (!IsInsideFrustum(*mesh))
			return;

		s_Data.ObjectData.Model = transform;
		s_Data.ObjectDataBuf->SetData(&s_Data.ObjectData, sizeof(RendererData::ObjectData));

		for (const SubMesh& submesh : mesh->GetSubMeshes())
		{
			if (submesh.Material)
				AssetManager::GetAsset<Texture2D>(submesh.Material)->Bind(1);
			RenderCommands::DrawIndexed(mesh->GetVA(), submesh.IndexCount, submesh.FirstIndex);
		}
	}



	void SceneRenderer::CalcFrustumPlanes(const glm::mat4& viewProj)
	{
		// Gribb - Hartmann method

		const glm::mat4 transposed = glm::transpose(viewProj);

		s_Data.FrustumPlanes[0].Normal = glm::vec3{ transposed[3] + transposed[0] }; // left
		s_Data.FrustumPlanes[0].Dist = transposed[3][3] + transposed[0][3];

		s_Data.FrustumPlanes[1].Normal = glm::vec3{ transposed[3] - transposed[0] }; // right
		s_Data.FrustumPlanes[1].Dist = transposed[3][3] - transposed[0][3];

		s_Data.FrustumPlanes[2].Normal = glm::vec3{ transposed[3] + transposed[1] }; // bottom
		s_Data.FrustumPlanes[2].Dist = transposed[3][3] + transposed[1][3];

		s_Data.FrustumPlanes[3].Normal = glm::vec3{ transposed[3] - transposed[1] }; // top
		s_Data.FrustumPlanes[3].Dist = transposed[3][3] - transposed[1][3];

		s_Data.FrustumPlanes[4].Normal = glm::vec3{ transposed[3] + transposed[2] }; // near
		s_Data.FrustumPlanes[4].Dist = transposed[3][3] + transposed[2][3];

		s_Data.FrustumPlanes[5].Normal = glm::vec3{ transposed[3] - transposed[2] }; // far
		s_Data.FrustumPlanes[5].Dist = transposed[3][3] - transposed[2][3];

		for (RendererData::Plane& p : s_Data.FrustumPlanes)
		{
			const float len = glm::length(p.Normal);
			p.Normal /= len;
			p.Dist /= len;
		}
	}

	bool SceneRenderer::IsInsideFrustum(const StaticMesh& mesh)
	{
		const BoundingSphere& bs{ mesh.GetBoundingSphere() };
		for (RendererData::Plane& p : s_Data.FrustumPlanes)
		{
			const float dist = glm::dot(p.Normal, bs.Center) + p.Dist;
			
			// (dist > bs.Radius) -> Inside
			if (dist < -bs.Radius)
				return false; // Outside the frustum

			// TODO: handle intersection (test bb, maybe check submeshes)
		}

		return true;
	}
}