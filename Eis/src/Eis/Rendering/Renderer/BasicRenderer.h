#pragma once

#include "Eis/Scene/Scene.h"

#include "Eis/Rendering/Objects/Shader.h"
#include "Eis/Rendering/Objects/Buffers.h"
#include "Eis/Rendering/Objects/Framebuffer.h"
#include "Eis/Rendering/Objects/EditorCamera.h"


namespace Eis
{
	class BasicRenderer
	{
	public:
		BasicRenderer();
		~BasicRenderer() = default;

		void SetScene(const Ref<Scene>& scene) { m_Scene = scene; }


		void Render();
		void RenderEditor(const EditorCamera& cam);


	private:
		Ref<Scene> m_Scene{};


		struct BasicRendererData
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
			Ref<UniformBuffer> SceneDataUniformBuf;

			struct ObjectData
			{
				glm::mat4 Model{};
			} ObjectData;
			Ref<UniformBuffer> ObjectDataBuf;

			Ref<Shader> MeshShader;

			Ref<Framebuffer> m_ScreenFB;
		} m_Data;
	};
}